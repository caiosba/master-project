/*
 # (C) 2009-2011 Jürgen Löb
 # jloeb@main-host.de
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 # See the Attribution-NonCommercial-ShareAlike 3.0 Unported for more details.
 #
 */

#include <iostream>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <math.h>
#include <typeinfo>
#include <algorithm>

#include <usb.h>
#include <libconfig.h++>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/uinput.h>

#include <cuse_lowlevel.h>
#include <fuse_opt.h>

#include "vrtypes.hpp"

#define mediansize 30
#define averagesize 30

using namespace std;

//usage
static const char
		*usage =
				"vr920 headtracking driver (C) 2009-2010 Juergen Loeb\n\n"
					"usage: vrtrack [options]\n"
					"\n"
					"options:\n"
					"    --help|-h|-?                print this help message\n"
					"    --device=DEVICENAME         customn device name (defaults to vrtrack)\n"
					"    --controldevice=DEVICENAME	 customn control device name\n"
					"                                (defaults to vrcontrol)\n"
					"    --multicast                 send tracking data via multicast\n"
					"    --udp                       send tracking data via udp\n"
					"    --mouse                     send tracking data via mouse emulation\n"
					"    --joystick                  send tracking data via joystick emulation\n"
					"    --scale=SCALE               scale movement (default: 1.0)\n"
					"    --invert-yaw|--invert-x     invert yaw/x-axis\n"
					"    --invert-pitch|--invert-y   invert pitch/y-axis\n"
					"    --invert-roll|--invert-z    invert roll/z-axis\n"
					"    --old-average               use old averaging algorithm\n"
					"                                (for compatibility only)\n"
					"    --mip=IP                    use customn ip address for udp multicasts\n"
					"                                (defaults to 224.0.0.42)\n"
					"    --mport=PORT                use customd port for udp multicasts\n"
					"                                (defaults to 4242)\n"
					"    --uip=IP                    use customn ip address for udp unicasts\n"
					"                                (defaults to 127.0.0.1)\n"
					"    --uport=PORT                use customd port for udp unicasts\n"
					"                                (defaults to 4242)\n"
					"    --callibration=CALLIB       use customn callibration file (default: ~/.vrtrack.cal)\n"
					"    --configuration=CONF        use customn configuation file (default: ~/.vrtrack.conf)\n"
					"    --delay=DELAY               use customn delay in main loop (default:1000)\n"
					"    -f                          stay in the foreground\n"
					"\n";

// defaults
static std::string mipaddress("224.0.0.42");
static unsigned mport = 4242;
static std::string uipaddress("127.0.0.1");
static unsigned uport = 4242;
static unsigned delay = 1000;
static unsigned ratio = 100; // cut off peaks
static bool average = false;
static bool calmode = false;
static int daemonize = true;
static bool invert_yaw = false;
static bool invert_pitch = false;
static bool invert_roll = false;
static double scale = 1.0f;
static std::string callibrationfile(".vrtrack.cal");
static std::string configurationfile(".vrtrack.conf");
static std::string dev_name("vrtrack");
static std::string cdev_name("vrcontrol");
static std::string device_name("no_dev");

//modes
enum Mode {
	MULTICAST = 1, UDP = 2, MOUSE = 4, JOYSTICK = 8, NONE = 0
};
char mode = 0;//MULTICAST|MOUSE|JOYSTICK|UDP;


//globals
static TrackingData trackingdata;
static libconfig::Config calibration;
static libconfig::Config configuration;
static Vec3d acc_old = (Vec3d) {0, 0, 0};
static Vec3d mag_old = (Vec3d) {0, 0, 0};

static Vec3d acc;
static Vec3d mag;

static double zero_yaw = 0;
static double zero_pitch = 0;
static double zero_roll = 0;

static Vec3d acc_max = (Vec3d) {0.1, 0.1, 0.1};
static Vec3d acc_min = (Vec3d) {-0.1, -0.1, -0.1};
static Vec3d mag_max = (Vec3d) {0.1, 0.1, 0.1};
static Vec3d mag_min = (Vec3d) {-0.1, -0.1, -0.1};

void manageModes(char from, char to);
void writecallibration();
void writeconfiguration();
void readcallibration();
void readconfiguration();
void zero();
void resetCallibration();

// fuse stuff
static void cusevr920_read(fuse_req_t req, size_t size, off_t off,
		struct fuse_file_info *fi);

cuse_lowlevel_ops vr920_clop;
cuse_lowlevel_ops vr920control_clop;

struct cusevr920_param {
	unsigned major;
	unsigned minor;
	char *dev_name;
	char *cdev_name;
	int is_help;
	float scale;
	char *uipaddress;
	unsigned uport;
	char *mipaddress;
	unsigned mport;
	char *callibrationfile;
	char *configurationfile;
	unsigned delay;
};

struct cusevr920_param vr920_param = { 0, 0, 0, 0, 0 };
struct cusevr920_param vr920control_param = { 0, 0, 0, 0, 0 };

#define VR920_OPT(t, p) { t, offsetof(struct cusevr920_param, p), 1 }

static const struct fuse_opt vr920_opts[]
= {
	VR920_OPT("--device=%s", dev_name),
	VR920_OPT("--controldevice=%s", cdev_name),
	FUSE_OPT_KEY("--multicast", 1),
	FUSE_OPT_KEY("--udp", 2),
	FUSE_OPT_KEY("--mouse", 3),
	FUSE_OPT_KEY("--joystick", 4),
	VR920_OPT("--scale=%f", scale),
	FUSE_OPT_KEY("--invert-yaw", 5),
	FUSE_OPT_KEY("--invert-pitch", 6),
	FUSE_OPT_KEY("--invert-roll", 7),
	FUSE_OPT_KEY("--invert-x", 5),
	FUSE_OPT_KEY("--invert-y", 6),
	FUSE_OPT_KEY("--invert-x", 7),
	FUSE_OPT_KEY("--old-average", 8),
	FUSE_OPT_KEY("-f", 9),
	VR920_OPT("--mip=%s", mipaddress),
	VR920_OPT("--mport=%u", mport),
	VR920_OPT("--uip=%s", uipaddress),
	VR920_OPT("--uport=%u", uport),
	VR920_OPT("--callibration=%s", callibrationfile),
	VR920_OPT("--configuration=%s", configurationfile),
	VR920_OPT("--delay=%u", delay),
	FUSE_OPT_KEY("-h", 0),
	FUSE_OPT_KEY("-?", 0),
	FUSE_OPT_KEY("--help", 0),
	NULL
};

static int process_args(void *data, const char *arg, int key,
		struct fuse_args *outargs) {
	struct cusevr920_param *param = (cusevr920_param *) data;

	(void) outargs;
	(void) arg;

	switch (key) {
	case 0:
		param->is_help = 1;
		fprintf(stderr, usage);
		exit(-1);
		return -1;
	case 1:
		mode = mode | MULTICAST;
		return 0;
		break;
	case 2:
		mode = mode | UDP;
		return 0;
		break;
	case 3:
		mode = mode | MOUSE;
		return 0;
		break;
	case 4:
		mode = mode | JOYSTICK;
		return 0;
		break;
	case 5:
		invert_yaw = true;
		return 0;
		break;
	case 6:
		invert_pitch = true;
		return 0;
		break;
	case 7:
		invert_roll = true;
		return 0;
		break;
	case 8:
		average = true;
		return 0;
		break;
	case 9:
		daemonize = false;
		return 0;
		break;
	default:
		fprintf(stderr, "unknown option: %s\n\n", arg);
		fprintf(stderr, usage);
		exit(-1);
		return 1;
	}
}
/*static const struct cuse_lowlevel_ops cusevr920_clop = {
 .read		= cusevr920_read,
 };*/

//FUSE stuff
static void cusevr920_read(fuse_req_t req, size_t size, off_t off,
		struct fuse_file_info *fi) {
	(void) fi;

	/*if (off >= cusexmp_size)
	 off = cusexmp_size;
	 if (size > cusexmp_size - off)
	 size = cusexmp_size - off;*/
	char readings[255];
	int length = sprintf(readings, "%f %f %f %f %f %f\n", trackingdata.yaw,
			trackingdata.pitch, trackingdata.roll, trackingdata.x,
			trackingdata.y, trackingdata.z);
	fuse_reply_buf(req, readings, length); //cusexmp_buf + off, size);
}

static void cusevr920control_read(fuse_req_t req, size_t size, off_t off,
		struct fuse_file_info *fi) {
	(void) fi;

	/*if (off >= cusexmp_size)
	 off = cusexmp_size;
	 if (size > cusexmp_size - off)
	 size = cusexmp_size - off;*/
	char readings[1024];
	int length = 0;
	if (!calmode)
		length
				= sprintf(
						readings,
						"name=%s,callibration=%u,joystick=%u,mouse=%u,udp=%u,multicast=%u,scale=%f,invert_yaw=%u,invert_pitch=%u,invert_roll=%u,mip=%s,mport=%u,uip=%s,uport=%u\n",
						device_name.c_str(), calmode,
						(mode & JOYSTICK) ? 1 : 0, (mode & MOUSE) ? 1 : 0,
						(mode & UDP) ? 1 : 0, (mode & MULTICAST) ? 1 : 0,
						scale, invert_yaw, invert_pitch, invert_roll,
						mipaddress.c_str(), mport, uipaddress.c_str(), uport);
	else
		length
				= sprintf(
						readings,
						"name=%s,callibration=%u,joystick=%u,mouse=%u,udp=%u,multicast=%u,scale=%f,invert_yaw=%u,invert_pitch=%u,invert_roll=%u,mip=%s,mport=%u,uip=%s,uport=%u,accellerometer.x=%f,accellerometer.y=%f,accellerometer.z=%f,magnetometer.x=%f,magnetometer.y=%f,magnetometer.z=%f,acc_min.x=%f,acc_min.y=%f,acc_min.z=%f,acc_max.x=%f,acc_max.y=%f,acc_max.z=%f,mag_min.x=%f,mag_min.y=%f,mag_min.z=%f,mag_max.x=%f,mag_max.y=%f,mag_max.z=%f\n",
						device_name.c_str(), calmode,
						(mode & JOYSTICK) ? 1 : 0, (mode & MOUSE) ? 1 : 0,
						(mode & UDP) ? 1 : 0, (mode & MULTICAST) ? 1 : 0,
						scale, invert_yaw, invert_pitch, invert_roll,
						mipaddress.c_str(), mport, uipaddress.c_str(), uport,
						acc.x, acc.y, acc.z, mag.x, mag.y, mag.z, acc_min.x,
						acc_min.y, acc_min.z, acc_max.x, acc_max.y, acc_max.z,
						mag_min.x, mag_min.y, mag_min.z, mag_max.x, mag_max.y,
						mag_max.z);
	fuse_reply_buf(req, readings, length); //cusexmp_buf + off, size);
}

bool comparecmd(const char* s1, const char* s2) {
	return strncmp(s1, s2, strlen(s2));
}

static void cusevr920control_write(fuse_req_t req, const char *buf,
		size_t size, off_t off, struct fuse_file_info *fi) {
	(void) fi;
	char command[255];
	command[size] = '\0';
	strncpy(command, buf + off, size);
	printf("%s %d %d\n", command, strlen(buf + off), (int) size);
	if (comparecmd(command, "callibration=0") == 0)
		calmode = 0;
	else if (comparecmd(command, "callibration=1") == 0)
		calmode = 1;
	else if (comparecmd(command, "joystick=0") == 0) {
		unsigned oldmode = mode;
		mode = mode & ~JOYSTICK;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "joystick=1") == 0) {
		unsigned oldmode = mode;
		mode = mode | JOYSTICK;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "mouse=0") == 0) {
		unsigned oldmode = mode;
		mode = mode & ~MOUSE;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "mouse=1") == 0) {
		unsigned oldmode = mode;
		mode = mode | MOUSE;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "udp=0") == 0) {
		unsigned oldmode = mode;
		mode = mode & ~UDP;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "udp=1") == 0) {
		unsigned oldmode = mode;
		mode = mode | UDP;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "multicast=0") == 0) {
		unsigned oldmode = mode;
		mode = mode & ~MULTICAST;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "multicast=1") == 0) {
		unsigned oldmode = mode;
		mode = mode | MULTICAST;
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "invert_yaw=0") == 0)
		invert_yaw = 0;
	else if (comparecmd(command, "invert_yaw=1") == 0)
		invert_yaw = 1;
	else if (comparecmd(command, "invert_pitch=0") == 0)
		invert_pitch = 0;
	else if (comparecmd(command, "invert_pitch=1") == 0)
		invert_pitch = 1;
	else if (comparecmd(command, "invert_roll=0") == 0)
		invert_roll = 0;
	else if (comparecmd(command, "invert_roll=1") == 0)
		invert_roll = 1;
	else if (comparecmd(command, "readcallibration") == 0)
		readcallibration();
	else if (comparecmd(command, "readconfiguration") == 0) {
		int oldmode = mode;
		readconfiguration();
		manageModes(oldmode, mode);
	} else if (comparecmd(command, "savecallibration") == 0)
		writecallibration();
	else if (comparecmd(command, "saveconfiguration") == 0)
		writeconfiguration();
	else if (comparecmd(command, "setzero") == 0)
		zero();
	else if (comparecmd(command, "reset") == 0)
		resetCallibration();
	else if (comparecmd(buf + off, "scale=") == 0) {
		scale = strtod(buf + off + 6, NULL);
		scale = (scale > 0) ? scale : 0;
	} else if (comparecmd(command, "mip=") == 0) {
		if (size > 20)
			fuse_reply_err(req, EIO);
		else {
			std::cout << mipaddress.c_str();
			mipaddress.erase();
			mipaddress.append(command + 4);
			mipaddress.erase('\n');
			//			strncpy(mipaddress, command+4,size-5);
			if (mode & MULTICAST) {
				manageModes(mode, mode ^ MULTICAST);
				manageModes(mode ^ MULTICAST, mode);
			}
		}
	} else if (comparecmd(command, "uip=") == 0) {
		if (size > 20)
			fuse_reply_err(req, EIO);
		else {
			uipaddress.erase();
			uipaddress.append(command + 4);
			uipaddress.erase('\n');
			//			strncpy(uipaddress, command+4,size-5);
			if (mode & UDP) {
				manageModes(mode, mode ^ UDP);
				manageModes(mode ^ UDP, mode);
			}
		}
	} else if (comparecmd(command, "mport=") == 0) {
		mport = atoi(command + 6);
		if (mode & MULTICAST) {
			manageModes(mode, mode ^ MULTICAST);
			manageModes(MULTICAST, mode ^ MULTICAST);
		}
	} else if (comparecmd(command, "uport=") == 0) {
		uport = atoi(command + 6);
		if (mode & UDP) {
			manageModes(mode, mode ^ UDP);
			manageModes(mode ^ UDP, mode);
		}
	} else
		fuse_reply_err(req, EIO);

	fuse_reply_write(req, size);
}

static void *trackingthreadfunc(void* args_in) {
	char name[128] = "DEVNAME=";
	strncat(name, dev_name.c_str(), sizeof(dev_name.c_str()) - 9);
	const char *dev_info_argv[] = { name };
	struct cuse_info ci;
	fuse_args args = *(fuse_args*) args_in;
	memset(&ci, 0, sizeof(ci));
	ci.dev_major = vr920_param.major;
	ci.dev_minor = vr920_param.minor;
	ci.dev_info_argc = 1;
	ci.dev_info_argv = dev_info_argv;
	ci.flags = CUSE_UNRESTRICTED_IOCTL;

	fuse_opt_add_arg(&args, "-f"); // prevent forking in fuse we don´t want to daemonize
	cuse_lowlevel_main(args.argc, args.argv, &ci, &vr920_clop, NULL);
	return NULL;
}

static void *controlthreadfunc(void* args_in) {
	char name[128] = "DEVNAME=";
	strncat(name, cdev_name.c_str(), sizeof(cdev_name.c_str()) - 9);
	const char *dev_info_argv[] = { name };
	struct cuse_info ci;
	fuse_args args = *(fuse_args*) args_in;
	memset(&ci, 0, sizeof(ci));
	ci.dev_major = vr920control_param.major;
	ci.dev_minor = vr920control_param.minor;
	ci.dev_info_argc = 1;
	ci.dev_info_argv = dev_info_argv;
	ci.flags = CUSE_UNRESTRICTED_IOCTL;

	fuse_opt_add_arg(&args, "-f"); // prevent forking in fuse we don´t want to daemonize
	cuse_lowlevel_main(args.argc, args.argv, &ci, &vr920control_clop, NULL);
	return NULL;
}

double acc_x[mediansize];
double acc_y[mediansize];
double acc_z[mediansize];

double mag_x[mediansize];
double mag_y[mediansize];
double mag_z[mediansize];

double sacc_x[mediansize];
double sacc_y[mediansize];
double sacc_z[mediansize];

double smag_x[mediansize];
double smag_y[mediansize];
double smag_z[mediansize];

double old_yaw = 0;
double old_pitch = 0;
double old_roll = 0;

int ufile;

struct uinput_user_dev uinp;
struct input_event event;

unsigned ringcounter = 0;

char key;
bool quit = false;

//daemonize
int daemon(int nochdir, int noclose) {
	int i = 0;
	switch (fork()) {
	case 0:
		break;
	case -1:
		return -1;
	default:
		_exit(0); /* exit the original process */
	}

	if (setsid() < 0) /* shoudn't fail */
		return -1;

	/* dyke out this switch if you want to acquire a control tty in */
	/* the future -- not normally advisable for daemons */

	switch (fork()) {
	case 0:
		break;
	case -1:
		return -1;
	default:
		_exit(0);
	}

	if (!nochdir)
		if (!chdir("/"))
			printf("chdir / failed!\n");

	if (!noclose) {
		for (; i < 255; ++i)
			close(i);
		open("/dev/null", O_RDWR);
		if (!dup(0))
			printf("dup failed!\n");
		if (!dup(0))
			printf("dup failed!\n");
	}

	return 0;
}

// network order for unicast
static void encodeFloat(char* buf, float val) {
	uint32_t tmp = htonl(*(uint32_t *) &val);
	memcpy(buf, &tmp, sizeof(uint32_t));
}

// network stuff
int multicast_socket_descriptor;
int unicast_socket_descriptor;
struct sockaddr_in multicast_address;
struct sockaddr_in unicast_address;

int createMulticastSocket() {
	multicast_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	if (multicast_socket_descriptor == -1) {
		perror("socket()");
		return EXIT_FAILURE;
	}
	memset(&multicast_address, 0, sizeof(multicast_address));
	multicast_address.sin_family = AF_INET;
	multicast_address.sin_addr.s_addr = inet_addr(mipaddress.c_str());
	multicast_address.sin_port = htons(mport);

	printf("Sending multicasts to %s:%d.\n", mipaddress.c_str(), mport);
	return 0;
}

void releaseMulticastSocket() {
	// shutdown socket
	shutdown(multicast_socket_descriptor, 2);
	// close socket
	close(multicast_socket_descriptor);
}

void writeMulticastSocket() {
	if (sendto(multicast_socket_descriptor, &trackingdata,
			sizeof(TrackingData), 0, (struct sockaddr *) &multicast_address,
			sizeof(multicast_address)) < 0) {
		perror("writeMulticastSocket()");
	}
}

int createUnicastSocket() {
	unicast_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	if (unicast_socket_descriptor == -1) {
		perror("socket()");
		return EXIT_FAILURE;
	}
	memset(&unicast_address, 0, sizeof(unicast_address));
	unicast_address.sin_family = PF_INET;
	unicast_address.sin_addr.s_addr = inet_addr(uipaddress.c_str());
	unicast_address.sin_port = htons(uport);

	printf("Sending unicasts to %s:%d.\n", uipaddress.c_str(), uport);
	return 0;
}

void releaseUnicastSocket() {
	// shutdown socket
	shutdown(unicast_socket_descriptor, 2);
	// close socket
	close(unicast_socket_descriptor);
}

void writeUnicastSocket() {
	const int packetSize = 1 + 6 * sizeof(float);
	char msg[packetSize];
	msg[0] = 1; // Version.
	encodeFloat(&msg[1], (float) trackingdata.yaw); // heading-deg
	encodeFloat(&msg[5], (float) trackingdata.roll); // roll-deg
	encodeFloat(&msg[9], (float) trackingdata.pitch); // pitch-deg
	encodeFloat(&msg[13], (float) 0); // pos-x (right)(dummy)
	encodeFloat(&msg[17], (float) 0); // pos-y (up)(dummy)
	encodeFloat(&msg[21], (float) 0); // pos-z (back)(dummy)

	if (sendto(unicast_socket_descriptor, &msg, sizeof(TrackingData), 0,
			(struct sockaddr *) &unicast_address, sizeof(unicast_address)) < 0) {
		perror("writeUnicastSocket()");

	}
}

// Mouse stuff
int createMouseDev() {
	system("modprobe -v uinput");

	int retcode, i;

	ufile = open("/dev/input/uinput", O_WRONLY | O_NDELAY);
	printf("open /dev/input/uinput returned %d.\n", ufile);
	if (ufile == 0) {
		printf("Could not open /dev/input/uinput retrying with /dev/uinput.\n");
		ufile = open("/dev/uinput", O_WRONLY | O_NDELAY);
		printf("open /dev/uinput returned %d.\n", ufile);
		if (ufile == 0) {
			printf("Could not open uinput.\n");
			return -1;
		}
	}

	memset(&uinp, 0, sizeof(uinp));
	strncpy(uinp.name, "vr920 mouse", 20);
	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;

	ioctl(ufile, UI_SET_EVBIT, EV_KEY);
	ioctl(ufile, UI_SET_EVBIT, EV_REL);
	ioctl(ufile, UI_SET_RELBIT, REL_X);
	ioctl(ufile, UI_SET_RELBIT, REL_Y);
	ioctl(ufile, UI_SET_RELBIT, REL_Z);

	for (i = 0; i < 256; i++) {
		ioctl(ufile, UI_SET_KEYBIT, i);
	}

	ioctl(ufile, UI_SET_KEYBIT, BTN_MOUSE);

	// create input device in input subsystem
	retcode = write(ufile, &uinp, sizeof(uinp));
	printf("First write returned %d.\n", retcode);

	retcode = (ioctl(ufile, UI_DEV_CREATE));
	printf("ioctl UI_DEV_CREATE returned %d.\n", retcode);
	if (retcode) {
		printf("Error create uinput device %d.\n", retcode);
		return -1;
	}
	return 0;
}

void releaseMouseDev() {
	ioctl(ufile, UI_DEV_DESTROY);
	close(ufile);
}

void writeMouseDev() {
	struct timeval tv1;
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_REL;
	event.code = REL_X;
	event.value = (trackingdata.yaw - old_yaw);
	if (!write(ufile, &event, sizeof(event)))
		return;
	old_yaw = trackingdata.yaw;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_REL;
	event.code = REL_Y;
	// reverse mouse for natural feeling
	event.value = (-trackingdata.pitch + old_pitch);
	if (!write(ufile, &event, sizeof(event)))
		return;
	old_pitch = trackingdata.pitch;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_REL;
	event.code = REL_Z;
	event.value = (trackingdata.roll - old_roll);
	if (!write(ufile, &event, sizeof(event)))
		return;
	old_roll = trackingdata.roll;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(ufile, &event, sizeof(event));

	// wait just a moment
	do {
		gettimeofday(&tv1, NULL);
	} while ((tv1.tv_usec & 0x3FFF) != 0);
	do {
		gettimeofday(&tv1, NULL);
	} while ((tv1.tv_usec & 0x3FFF) == 0);
}

// Joystick stuff
int createJoyDev() {
	system("modprobe -v uinput");
	int retcode, i;

	ufile = open("/dev/input/uinput", O_WRONLY | O_NDELAY);
	printf("open /dev/input/uinput returned %d.\n", ufile);
	if (ufile == 0) {
		printf("Could not open uinput.\n");
		return -1;
	}

	memset(&uinp, 0, sizeof(uinp));
	strncpy(uinp.name, "vr920 joystick emulation", UINPUT_MAX_NAME_SIZE);
	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;
	uinp.id.vendor = 0x00046d; // Logitech
	uinp.id.product = 0;

	ioctl(ufile, UI_SET_EVBIT, EV_SYN);
	ioctl(ufile, UI_SET_EVBIT, EV_ABS);
	ioctl(ufile, UI_SET_EVBIT, EV_KEY);

	ioctl(ufile, UI_SET_ABSBIT, ABS_X);
	ioctl(ufile, UI_SET_ABSBIT, ABS_Y);
	ioctl(ufile, UI_SET_ABSBIT, ABS_Z);

	for (i = 0; i < ABS_MAX; ++i) {
		uinp.absmax[i] = 511;
		uinp.absmin[i] = -512;
	}

	// Create device
	write(ufile, &uinp, sizeof(uinp));

	retcode = ioctl(ufile, UI_DEV_CREATE);

	printf("ioctl UI_DEV_CREATE returned %d.\n", retcode);
	if (retcode) {
		printf("Error create uinput device %d.\n", retcode);
		return -1;
	}
	return 0;

}

void releaseJoyDev() {
	ioctl(ufile, UI_DEV_DESTROY);
	close(ufile);
}

void writeJoyDev() {
	struct timeval tv1;
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_ABS;
	event.code = ABS_X;
	event.value = (trackingdata.yaw);
	if (!write(ufile, &event, sizeof(event)))
		return;
	old_yaw = trackingdata.yaw;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_ABS;
	event.code = ABS_Y;
	event.value = (trackingdata.pitch);
	if (!write(ufile, &event, sizeof(event)))
		return;
	old_pitch = trackingdata.pitch;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_ABS;
	event.code = ABS_Z;
	event.value = (trackingdata.roll);
	if (!write(ufile, &event, sizeof(event)))
		return;
	old_roll = trackingdata.roll;

	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(ufile, &event, sizeof(event));

	// wait just a moment
	do {
		gettimeofday(&tv1, NULL);
	} while ((tv1.tv_usec & 0x3FFF) != 0);
	do {
		gettimeofday(&tv1, NULL);
	} while ((tv1.tv_usec & 0x3FFF) == 0);
}

void readconfiguration() {
	try {
		configuration.readFile(configurationfile.c_str());
	} catch (libconfig::ParseException e) {
		std::cout << "Error parsing calibration file " << configurationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
		std::cout << ((libconfig::ParseException) e).getError() << " in line "
				<< ((libconfig::ParseException) e).getLine() << std::endl;
	} catch (libconfig::ConfigException e) {

		std::cout << "Error loading calibration file " << configurationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
	}

	if (!configuration.lookupValue("device", dev_name))
		std::cout << "Error loading device from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("controldevice", cdev_name))
		std::cout << "Error loading controldevice from configuration file!!!"
				<< std::endl;
	bool cmode = 0;
	if (!configuration.lookupValue("multicast", cmode))
		std::cout << "Error loading multicast from configuration file!!!"
				<< std::endl;
	if (cmode)
		mode = mode | MULTICAST;
	cmode = 0;
	if (!configuration.lookupValue("udp", cmode))
		std::cout << "Error loading udp from configuration file!!!"
				<< std::endl;
	if (cmode)
		mode = mode | UDP;
	cmode = 0;
	if (!configuration.lookupValue("mouse", cmode))
		std::cout << "Error loading mouse from configuration file!!!"
				<< std::endl;
	if (cmode)
		mode = mode | MOUSE;
	cmode = 0;
	if (!configuration.lookupValue("joystick", cmode))
		std::cout << "Error loading joystick from configuration file!!!"
				<< std::endl;
	if (cmode)
		mode = mode | JOYSTICK;
	if (!configuration.lookupValue("scale", scale))
		std::cout << "Error loading scale from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("invert-yaw", invert_yaw))
		std::cout << "Error loading invert-yaw from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("invert-pitch", invert_pitch))
		std::cout << "Error loading invert-pitch from configuration file!!!"
				<< std::endl;

	if (!configuration.lookupValue("invert-roll", invert_roll))
		std::cout << "Error loading invert-roll from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("mip", mipaddress))
		std::cout << "Error loading mip from calibration file!!!" << std::endl;

	if (!configuration.lookupValue("mport", mport))
		std::cout << "Error loading mport from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("uip", uipaddress))
		std::cout << "Error loading uiph from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("uport", uport))
		std::cout << "Error loading uport from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("callibration", callibrationfile))
		std::cout << "Error loading callibration from configuration file!!!"
				<< std::endl;
	if (!configuration.lookupValue("delay", delay))
		std::cout << "Error loading zdelay from configuration file!!!"
				<< std::endl;

}

void writeconfiguration() {
	libconfig::Setting &setting = configuration.getRoot();
	if (setting.exists("device"))
		setting.remove("device");
	setting.add("device", libconfig::Setting::TypeString) = dev_name;
	if (setting.exists("controldevice"))
		setting.remove("controldevice");
	setting.add("controldevice", libconfig::Setting::TypeString) = cdev_name;
	if (setting.exists("multicast"))
		setting.remove("multicast");
	setting.add("multicast", libconfig::Setting::TypeBoolean) = (bool) (mode
			& MULTICAST ? 1 : 0);
	if (setting.exists("udp"))
		setting.remove("udp");
	setting.add("udp", libconfig::Setting::TypeBoolean)
			= (bool) (mode & UDP ? 1 : 0);
	if (setting.exists("mouse"))
		setting.remove("mouse");
	setting.add("mouse", libconfig::Setting::TypeBoolean) = (bool) (mode
			& MOUSE ? 1 : 0);
	if (setting.exists("joystick"))
		setting.remove("joystick");
	setting.add("joystick", libconfig::Setting::TypeBoolean) = (bool) (mode
			& JOYSTICK ? 1 : 0);
	if (setting.exists("scale"))
		setting.remove("scale");
	setting.add("scale", libconfig::Setting::TypeFloat) = (float) scale;
	if (setting.exists("invert-yaw"))
		setting.remove("invert-yaw");
	setting.add("invert-yaw", libconfig::Setting::TypeBoolean)
			= (bool) invert_yaw;
	if (setting.exists("invert-pitch"))
		setting.remove("invert-pitch");
	setting.add("invert-pitch", libconfig::Setting::TypeBoolean)
			= (bool) invert_pitch;
	if (setting.exists("invert-roll"))
		setting.remove("invert-roll");
	setting.add("invert-roll", libconfig::Setting::TypeBoolean)
			= (bool) invert_roll;
	if (setting.exists("old-average"))
		setting.remove("old-average");
	setting.add("old-average", libconfig::Setting::TypeBoolean)
			= (bool) average;
	if (setting.exists("mip"))
		setting.remove("mip");
	setting.add("mip", libconfig::Setting::TypeString) = mipaddress;
	if (setting.exists("mport"))
		setting.remove("mport");
	setting.add("mport", libconfig::Setting::TypeInt) = (int) mport;

	if (setting.exists("uip"))
		setting.remove("uip");
	setting.add("uip", libconfig::Setting::TypeString) = uipaddress;
	if (setting.exists("uport"))
		setting.remove("uport");
	setting.add("uport", libconfig::Setting::TypeInt) = (int) uport;
	if (setting.exists("callibration"))
		setting.remove("callibration");
	setting.add("callibration", libconfig::Setting::TypeString)
			= callibrationfile;
	if (setting.exists("delay"))
		setting.remove("delay");
	setting.add("delay", libconfig::Setting::TypeInt) = (int) delay;

	try {
		configuration.writeFile(configurationfile.c_str());
	} catch (libconfig::ParseException e) {
		std::cout << "Error writing configuration file " << configurationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
		std::cout << ((libconfig::ParseException) e).getError() << " in line "
				<< ((libconfig::ParseException) e).getLine() << std::endl;

	} catch (libconfig::ConfigException e) {
		std::cout << "Error writing configuration file " << configurationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
	}

}

// callibration stuff

void resetCallibration() {
	acc_max = (Vec3d) {0.1, 0.1, 0.1};
	acc_min = (Vec3d) {-0.1, -0.1, -0.1};
	mag_max = (Vec3d) {0.1, 0.1, 0.1};
	mag_min = (Vec3d) {-0.1, -0.1, -0.1};
	zero_yaw = 0;
	zero_pitch = 0;
	zero_roll = 0;
}

void readDouble(libconfig::Config* configuration, std::string param,
		double* val) {
	if (!configuration->lookupValue(param, *val))
		std::cout << "Error loading " << param << " from calibration file!!!"
				<< std::endl;
}
void readcallibration() {
	try {
		calibration.readFile(callibrationfile.c_str());
	} catch (libconfig::ParseException e) {
		std::cout << "Error parsing calibration file " << callibrationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
		std::cout << ((libconfig::ParseException) e).getError() << " in line "
				<< ((libconfig::ParseException) e).getLine() << std::endl;
	} catch (libconfig::ConfigException e) {

		std::cout << "Error loading calibration file " << callibrationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
	}
	std::string param;
	param = device_name + ".acc_max_x";
	readDouble(&calibration, param, &acc_max.x);
	param = device_name + ".acc_max_y";
	readDouble(&calibration, param, &acc_max.y);
	param = device_name + ".acc_max_z";
	readDouble(&calibration, param, &acc_max.z);
	param = device_name + ".acc_min_x";
	readDouble(&calibration, param, &acc_min.x);
	param = device_name + ".acc_min_y";
	readDouble(&calibration, param, &acc_min.y);
	param = device_name + ".acc_min_z";
	readDouble(&calibration, param, &acc_min.z);
	param = device_name + ".mag_max_x";
	readDouble(&calibration, param, &mag_max.x);
	param = device_name + ".mag_max_y";
	readDouble(&calibration, param, &mag_max.y);
	param = device_name + ".mag_max_z";
	readDouble(&calibration, param, &mag_max.z);
	param = device_name + ".mag_min_x";
	readDouble(&calibration, param, &mag_min.x);
	param = device_name + ".mag_min_y";
	readDouble(&calibration, param, &mag_min.y);
	param = device_name + ".mag_min_z";
	readDouble(&calibration, param, &mag_min.z);

	param = device_name + ".zero_yaw";
	readDouble(&calibration, param, &zero_yaw);
	param = device_name + ".zero_pitch";
	readDouble(&calibration, param, &zero_pitch);
	param = device_name + ".zero_roll";
	readDouble(&calibration, param, &zero_roll);

}
void setAddDouble(libconfig::Setting &setting, std::string param, double val) {
	if (setting.exists(param))
		setting.remove(param);
	setting.add(param, libconfig::Setting::TypeFloat) = val;
}

void writecallibration() {
	libconfig::Setting &setting = calibration.getRoot();

	if (setting.exists(device_name.c_str()))
		setting.remove(device_name.c_str());
	libconfig::Setting &group = setting.add(device_name.c_str(),
			libconfig::Setting::TypeGroup);
	setAddDouble(group, "acc_max_x", acc_max.x);
	setAddDouble(group, "acc_max_y", acc_max.y);
	setAddDouble(group, "acc_max_z", acc_max.z);
	setAddDouble(group, "acc_min_x", acc_min.x);
	setAddDouble(group, "acc_min_y", acc_min.y);
	setAddDouble(group, "acc_min_z", acc_min.z);
	setAddDouble(group, "mag_max_x", mag_max.x);
	setAddDouble(group, "mag_max_y", mag_max.y);
	setAddDouble(group, "mag_max_z", mag_max.z);
	setAddDouble(group, "mag_min_x", mag_min.x);
	setAddDouble(group, "mag_min_y", mag_min.y);
	setAddDouble(group, "mag_min_z", mag_min.z);

	setAddDouble(group, "zero_yaw", zero_yaw);
	setAddDouble(group, "zero_pitch", zero_pitch);
	setAddDouble(group, "zero_roll", zero_roll);

	try {
		calibration.writeFile(callibrationfile.c_str());
	} catch (libconfig::ParseException e) {
		std::cout << "Error writing calibration file " << callibrationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
		std::cout << ((libconfig::ParseException) e).getError() << " in line "
				<< ((libconfig::ParseException) e).getLine() << std::endl;

	} catch (libconfig::ConfigException e) {
		std::cout << "Error writing calibration file " << callibrationfile
				<< " !!!" << std::endl;
		std::cout << typeid(e).name() << std::endl;
	}

}

void zero() {
	zero_yaw = trackingdata.yaw + zero_yaw;
	zero_pitch = trackingdata.pitch + zero_pitch;
	zero_roll = trackingdata.roll + zero_roll;
}

// Signals handler

void catchQuit(int sig) {
	printf("Caught signal %d Shutting down.\n", sig);
	quit = true;
}

// USB Stuff

struct usb_device *find_device(int vendor, int product) {
	struct usb_bus *bus;

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		struct usb_device *dev;

		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == vendor && dev->descriptor.idProduct
					== product)
				return dev;
		}
	}
	return NULL;
}

// open/close devices/sockets according to mode
void manageModes(char from, char to) {
	if (!(from & MOUSE) && to & MOUSE)
		createMouseDev();
	if (from & MOUSE && !(to & MOUSE))
		releaseMouseDev();
	if (!(from & JOYSTICK) && to & JOYSTICK)
		createJoyDev();
	if (from & JOYSTICK && !(to & JOYSTICK))
		releaseJoyDev();
	if (!(from & UDP) && to & UDP)
		createUnicastSocket();
	if (from & UDP && !(to & UDP))
		releaseUnicastSocket();
	if (!(from & MULTICAST) && to & MULTICAST)
		createMulticastSocket();
	if (from & MULTICAST && !(to & MULTICAST))
		releaseMulticastSocket();
}

int main(int argc, char *argv[]) {
	pthread_t trackingthread;
	pthread_t controlthread;
	system("modprobe -v cuse");
	vr920_clop.read = cusevr920_read;
	vr920control_clop.read = cusevr920control_read;
	vr920control_clop.write = cusevr920control_write;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	if (fuse_opt_parse(&args, &vr920_param, vr920_opts, process_args)
			|| fuse_opt_parse(&args, &vr920control_param, vr920_opts,
					process_args)) {
		printf("failed to parse option\n");
		return 1;
	}

	if (vr920_param.mipaddress)
		mipaddress = vr920_param.mipaddress;
	if (vr920_param.uipaddress)
		uipaddress = vr920_param.uipaddress;
	if (vr920_param.mport)
		mport = vr920_param.mport;
	if (vr920_param.uport)
		uport = vr920_param.uport;
	if (vr920_param.dev_name)
		dev_name = vr920_param.dev_name;
	if (vr920_param.cdev_name)
		cdev_name = vr920_param.cdev_name;

	if (vr920_param.scale > 0)
		scale = vr920_param.scale;
	if (daemonize)
		daemon(0, 1);

	pthread_create(&trackingthread, NULL, trackingthreadfunc, &args);
	pthread_create(&controlthread, NULL, controlthreadfunc, &args);
	//fusethreadfunc(&ftargs);
	std::string conffilename(getenv("HOME"));
	if (configurationfile[0] != '/') {

		conffilename.append("/");
		conffilename.append(configurationfile);
		configurationfile = (char*) conffilename.c_str();
	}
	std::cout << "Using configurationfile:" << configurationfile << std::endl;

	readconfiguration();

	manageModes(0, mode);

	// Setup signal handler

	signal(SIGTERM, catchQuit);
	signal(SIGQUIT, catchQuit);
	signal(SIGABRT, catchQuit);
	signal(SIGINT, catchQuit);

	// Initialize the USB device.

	int ret, vendor, product;
	struct usb_device *dev;
	struct usb_dev_handle *devh;
	char buf[65535];

	vendor = 0x1bae;
	product = 0x014b;

	while (!quit) {
		usb_init();
		usb_set_debug(255);
		usb_find_busses();
		usb_find_devices();
		dev = find_device(vendor, product);
		if (dev == NULL) {
			std::cout << "No VR920 attached to this system found." << endl;

			//exit(1);
		} else {

			devh = usb_open(dev);

			if (!usb_get_driver_np(devh, 3, buf, sizeof(buf))) {
				printf(
						"Looks like the USB HID driver has claimed it. Detaching it.\n");
				ret = usb_detach_kernel_driver_np(devh, 3);
			}

			ret = usb_set_configuration(devh, 0x0000001);

			ret = usb_claim_interface(devh, 3);
			if (ret != 0) {
				usb_release_interface(devh, 3);
				usb_close(devh);
				std::cout << "Couldn't claim interface: Error " << ret << endl;
			}

			int send = 0;
			printf("try\n");
			if (ret >= 0) {
				fprintf(stderr, "VR920 connected.\n");
				device_name = ("VR920");
			}

			std::string calfilename(getenv("HOME"));
			if (callibrationfile[0] != '/') {

				calfilename.append("/");
				calfilename.append(callibrationfile);
				callibrationfile = (char*) calfilename.c_str();
			}
			std::cout << "Using callibrationfile:" << callibrationfile
					<< std::endl;

			readcallibration();

			while (!quit && ret >= 0) { // reconnect on usb error or quit
				send = (send + 1) % 5;

				// Update the VR920 data

				ret
						= usb_interrupt_read(devh, 0x00000083, buf, 0x0000010,
								1000);

				acc.x = double(*(short*) (buf + 3));
				acc.z = double(*(short*) (buf + 5));
				acc.y = double(*(short*) (buf + 7));
				mag.y = -double(*(short*) (buf + 9));
				mag.z = -double(*(short*) (buf + 11));
				mag.x = -double(*(short*) (buf + 13));

				if (calmode) {
					//minmax
					if (acc.x > acc_max.x)
						acc_max.x = acc.x;
					if (acc.x < acc_min.x)
						acc_min.x = acc.x;
					if (acc.y > acc_max.y)
						acc_max.y = acc.y;
					if (acc.y < acc_min.y)
						acc_min.y = acc.y;
					if (acc.z > acc_max.z)
						acc_max.z = acc.z;
					if (acc.z < acc_min.z)
						acc_min.z = acc.z;

					if (mag.x > mag_max.x)
						mag_max.x = mag.x;
					if (mag.x < mag_min.x)
						mag_min.x = mag.x;
					if (mag.y > mag_max.y)
						mag_max.y = mag.y;
					if (mag.y < mag_min.y)
						mag_min.y = mag.y;
					if (mag.z > mag_max.z)
						mag_max.z = mag.z;
					if (mag.z < mag_min.z)
						mag_min.z = mag.z;
				}

				//normalize
				acc.x = ((acc.x - acc_min.x - (acc_max.x - acc_min.x) / 2)
						/ (acc_max.x - acc_min.x) * 2);
				acc.y = ((acc.y - acc_min.y - (acc_max.y - acc_min.y) / 2)
						/ (acc_max.y - acc_min.y) * 2);
				acc.z = ((acc.z - acc_min.z - (acc_max.z - acc_min.z) / 2)
						/ (acc_max.z - acc_min.z) * 2);
				mag.x = ((mag.x - mag_min.x - (mag_max.x - mag_min.x) / 2)
						/ (mag_max.x - mag_min.x) * 2);
				mag.y = ((mag.y - mag_min.y - (mag_max.y - mag_min.y) / 2)
						/ (mag_max.y - mag_min.y) * 2);
				mag.z = ((mag.z - mag_min.z - (mag_max.z - mag_min.z) / 2)
						/ (mag_max.z - mag_min.z) * 2);

				if (average) { // old average value
					if (fabs(fabs(acc.x) - fabs(acc_old.x)) > (acc_max.x
							- acc_min.x) / ratio)
						acc.x = acc_old.x;
					else {
						acc.x = (acc_old.x * 39 + acc.x) / 40;
						acc_old.x = acc.x;
					}
					if (fabs(fabs(acc.y) - fabs(acc_old.y)) > (acc_max.y
							- acc_min.y) / ratio)
						acc.y = acc_old.y;
					else {
						acc.y = (acc_old.y * 39 + acc.y) / 40;
						acc_old.y = acc.y;
					}
					if (fabs(fabs(acc.z) - fabs(acc_old.z)) > (acc_max.z
							- acc_min.z) / ratio)
						acc.z = acc_old.z;
					else {
						acc.z = (acc_old.z * 39 + acc.z) / 40;
						acc_old.z = acc.z;
					}
					if (fabs(fabs(mag.x) - fabs(mag_old.x)) > (mag_max.x
							- mag_min.x) / ratio)
						mag.x = mag_old.x;
					else {
						mag.x = (mag_old.x * 39 + mag.x) / 40;
						mag_old.x = mag.x;
					}
					if (fabs(fabs(mag.y) - fabs(mag_old.y)) > (mag_max.y
							- mag_min.y) / ratio)
						mag.y = mag_old.y;
					else {
						mag.y = (mag_old.y * 39 + mag.y) / 40;
						mag_old.y = mag.y;
					}
					if (fabs(fabs(mag.z) - fabs(mag_old.z)) > (mag_max.z
							- mag_min.z) / ratio)
						mag.z = mag_old.z;
					else {
						mag.z = (mag_old.z * 39 + mag.z) / 40;
						mag_old.z = mag.z;
					}
				} else { // new median value


					acc.x = acc.x;
					acc.y = acc.y;
					acc.z = acc.z;

					mag.x = mag.x;
					mag.y = mag.y;
					mag.z = mag.z;

					ringcounter = (ringcounter + 1) % mediansize;

					acc_x[ringcounter] = acc.x;
					acc_y[ringcounter] = acc.y;
					acc_z[ringcounter] = acc.z;

					mag_x[ringcounter] = mag.x;
					mag_y[ringcounter] = mag.y;
					mag_z[ringcounter] = mag.z;

					for (int i = 0; i < mediansize; i++) {
						sacc_x[i] = acc_x[i];
						sacc_y[i] = acc_y[i];
						sacc_z[i] = acc_z[i];

						smag_x[i] = mag_x[i];
						smag_y[i] = mag_y[i];
						smag_z[i] = mag_z[i];
					}
					sort(sacc_x, sacc_x + mediansize);
					sort(sacc_y, sacc_y + mediansize);
					sort(sacc_z, sacc_z + mediansize);
					sort(smag_x, smag_x + mediansize);
					sort(smag_y, smag_y + mediansize);
					sort(smag_z, smag_z + mediansize);
					//for(int i=0;i<mediansize;i++) std::cout <<sacc_x[i]<<":"<<sacc_y[i]<<":"<<sacc_z[i]<<":"<<smag_x[i]<<":"<<smag_y[i]<<":"<<smag_z[i]<<std::endl;

					acc.x = (sacc_x[mediansize / 2]
							+ sacc_x[mediansize / 2 - 1]) / 2;
					acc.y = (sacc_y[mediansize / 2]
							+ sacc_y[mediansize / 2 - 1]) / 2;
					acc.z = (sacc_z[mediansize / 2]
							+ sacc_z[mediansize / 2 - 1]) / 2;
					mag.x = (smag_x[mediansize / 2]
							+ smag_x[mediansize / 2 - 1]) / 2;
					mag.y = (smag_y[mediansize / 2]
							+ smag_y[mediansize / 2 - 1]) / 2;
					mag.z = (smag_z[mediansize / 2]
							+ smag_z[mediansize / 2 - 1]) / 2;

					//additional average


					acc.x = (acc_old.x * (averagesize - 1) + acc.x)
							/ averagesize;
					acc_old.x = acc.x;

					acc.y = (acc_old.y * (averagesize - 1) + acc.y)
							/ averagesize;
					acc_old.y = acc.y;

					acc.z = (acc_old.z * (averagesize - 1) + acc.z)
							/ averagesize;
					acc_old.z = acc.z;

					mag.x = (mag_old.x * (averagesize - 1) + mag.x)
							/ averagesize;
					mag_old.x = mag.x;

					mag.y = (mag_old.y * (averagesize - 1) + mag.y)
							/ averagesize;
					mag_old.y = mag.y;

					mag.z = (mag_old.z * (averagesize - 1) + mag.z)
							/ averagesize;
					mag_old.z = mag.z;

				}

				//double yaw;
				//double mag_pitch;
				//double mag_roll;
				double acc_pitch;
				double acc_roll;

				acc_roll = atan2(acc.x, sqrt(acc.y * acc.y + acc.z * acc.z))
						* 180 / M_PI;
				//acc_pitch=atan2(acc.y, sqrt(acc.x*acc.x + acc.z*acc.z))*180/M_PI;
				acc_pitch = atan2(acc.y, sqrt(acc.x * acc.x + acc.z * acc.z))
						* 180 / M_PI;

				//double xh= mag.x*cos((-acc_roll)/180.0*M_PI)+mag.y*sin(-acc_roll/180.0*M_PI)*sin((acc_pitch)/180.0*M_PI)-mag.z*sin(-acc_roll/180.0*M_PI)*cos((acc_pitch)/180.0*M_PI);
				//double yh= mag.y*cos(acc_pitch/180.0*M_PI)+mag.z*sin(acc_pitch/180.0*M_PI);


				double xh = mag.x * cos((-acc_pitch) / 180.0 * M_PI) + mag.y
						* sin(acc_roll / 180.0 * M_PI) * sin((-acc_pitch)
						/ 180.0 * M_PI) - mag.z
						* sin(-acc_pitch / 180.0 * M_PI) * cos((-acc_roll)
						/ 180.0 * M_PI);
				double yh = mag.y * cos(acc_roll / 180.0 * M_PI) + mag.z * sin(
						acc_roll / 180.0 * M_PI);

				double length = sqrt(xh * xh + yh * yh);
				xh = xh / length;
				yh = yh / length;

				trackingdata.yaw = -scale * atan2(xh, yh) * 180 / M_PI
						- zero_yaw;
				trackingdata.roll = scale * acc_roll - zero_roll;
				trackingdata.pitch = scale * acc_pitch - zero_pitch;

				if (invert_yaw)
					trackingdata.yaw = -trackingdata.yaw;
				if (invert_pitch)
					trackingdata.pitch = -trackingdata.pitch;
				if (invert_roll)
					trackingdata.roll = -trackingdata.roll;

				double A = cos(-trackingdata.pitch / 180.0 * M_PI);
				double B = sin(-trackingdata.pitch / 180.0 * M_PI);
				double C = cos(-trackingdata.roll / 180.0 * M_PI);
				double D = sin(-trackingdata.roll / 180.0 * M_PI);
				double E = cos(trackingdata.yaw / 180.0 * M_PI);
				double F = sin(trackingdata.yaw / 180.0 * M_PI);
				double AD = A * D;
				double BD = B * D;

				trackingdata.viewmatrix[0] = C * E;
				trackingdata.viewmatrix[1] = -C * F;
				trackingdata.viewmatrix[2] = D;
				trackingdata.viewmatrix[3] = 0;
				trackingdata.viewmatrix[4] = BD * E + A * F;
				trackingdata.viewmatrix[5] = -BD * F + A * E;
				trackingdata.viewmatrix[6] = -B * C;
				trackingdata.viewmatrix[7] = 0;
				trackingdata.viewmatrix[8] = -AD * E + B * F;
				trackingdata.viewmatrix[9] = AD * F + B * E;
				trackingdata.viewmatrix[10] = A * C;
				trackingdata.viewmatrix[11] = 0;
				trackingdata.viewmatrix[12] = 0;
				trackingdata.viewmatrix[13] = 0;
				trackingdata.viewmatrix[14] = 0;
				trackingdata.viewmatrix[15] = 1;

				if (!send) {
					if (mode & MOUSE)
						writeMouseDev();
					if (mode & JOYSTICK)
						writeJoyDev();
					if (mode & UDP)
						writeUnicastSocket();
					if (mode & MULTICAST)
						writeMulticastSocket();

				}

				usleep(delay); // Don't max out CPU
			}
		}
		if (!quit) {
			fprintf(stderr,
					"VR920 not found. Trying to reconnect in 1 second.\n");
			usleep(1000000); // wait 1s with reconnect try
		} else {
			usb_release_interface(devh, 3);
			usb_close(devh);
		}

	}
	manageModes(mode, 0);

	return 0;
}

