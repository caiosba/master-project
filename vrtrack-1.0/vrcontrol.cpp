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
#include <signal.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "vrtypes.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

char key;
bool quit = false;

char *dev_name = "/dev/vrtrack";
char *cdev_name = "/dev/vrcontrol";
FILE* controldevice;
FILE* trackingdevice;

char buffer[1024];

bool calmode = false;
bool mouse = false;
bool joystick = false;
bool multicast = false;
bool udp = false;

bool invert_yaw = false;
bool invert_pitch = false;
bool invert_roll = false;

unsigned mport = 4242;
unsigned uport = 4242;
static std::string mipaddress("224.0.0.42");
static std::string uipaddress("127.0.0.1");
static std::string device_name("no_dev");

double scale = -1.0f;

int delay = 1000;

Vec3d acc_max = (Vec3d) {0.1, 0.1, 0.1};
Vec3d acc_min = (Vec3d) {-0.1, -0.1, -0.1};
Vec3d mag_max = (Vec3d) {0.1, 0.1, 0.1};
Vec3d mag_min = (Vec3d) {-0.1, -0.1, -0.1};

Vec3d acc = (Vec3d) {1.0, 1.0, 1.0};
;
Vec3d mag = (Vec3d) {1.0, 1.0, 1.0};
;

double zero_yaw = 0;
double zero_pitch = 0;
double zero_roll = 0;

TrackingData trackingdata;

void opendevs() {
	if ((trackingdevice = fopen(dev_name, "r")) == NULL) {
		printf("Error openning tracking device file: %s\n", dev_name);
		exit(1);
	} else {
		//printf("Device file opened\n");
	}
	if ((controldevice = fopen(cdev_name, "a+")) == NULL) {
		printf("Error openning control device file: %s\n", cdev_name);
		exit(1);
	} else {
		//printf("Device file opened\n");
	}

}

void closedevs() {
	fclose(trackingdevice);
	fclose(controldevice);
}

void readtrackingdev() {
	unsigned i = 0;
	while ((buffer[i] = getc(trackingdevice)) != '\n' && i < 255) // loop for reading a char upto end of line
	{
		i++;
	}
	//putchar('*');
	//printf("%s",buffer);
	char *start = buffer;
	//if((start=strtok(NULL," "))!=NULL)
	//printf("%s", start);
	trackingdata.yaw = strtod(start, &start);
	//if((start=strtok(NULL," "))!=NULL)
	trackingdata.pitch = strtod(start, &start);
	//if((start=strtok(NULL," "))!=NULL)
	trackingdata.roll = strtod(start, &start);
	//if((start=strtok(NULL," "))!=NULL)
	trackingdata.x = strtod(start, &start);
	//if((start=strtok(NULL," "))!=NULL)
	trackingdata.y = strtod(start, &start);
	//ff((start=strtok(NULL," "))!=NULL)
	trackingdata.z = strtod(start, &start);

}

void readcontroldev() {
	unsigned i = 0;
	while ((buffer[i] = getc(controldevice)) != '\n' && i < 1024) // loop for reading a char upto end of line
	{
		i++;
	}
	//putchar('*');
	//printf("%s",buffer);
	char* token;
	if ((token = strtok(buffer, ",\n")) != NULL)
		do {
			//printf("A%sE", token);
			if (strncmp(token, "name=", 4) == 0) {
				device_name = (token + 5);
			} else if (strcmp(token, "callibration=0") == 0) {
				if (calmode == 1)
					clear();
				calmode = 0;
			} else if (strcmp(token, "callibration=1") == 0)
				calmode = 1;
			else if (strcmp(token, "joystick=0") == 0)
				joystick = 0;
			else if (strcmp(token, "joystick=1") == 0)
				joystick = 1;
			else if (strcmp(token, "mouse=0") == 0)
				mouse = 0;
			else if (strcmp(token, "mouse=1") == 0)
				mouse = 1;
			else if (strcmp(token, "udp=0") == 0)
				udp = 0;
			else if (strcmp(token, "udp=1") == 0)
				udp = 1;
			else if (strcmp(token, "multicast=0") == 0)
				multicast = 0;
			else if (strcmp(token, "multicast=1") == 0)
				multicast = 1;
			else if (strcmp(token, "invert_yaw=0") == 0)
				invert_yaw = 0;
			else if (strcmp(token, "invert_yaw=1") == 0)
				invert_yaw = 1;
			else if (strcmp(token, "invert_pitch=0") == 0)
				invert_pitch = 0;
			else if (strcmp(token, "invert_pitch=1") == 0)
				invert_pitch = 1;
			else if (strcmp(token, "invert_roll=0") == 0)
				invert_roll = 0;
			else if (strcmp(token, "invert_roll=1") == 0)
				invert_roll = 1;
			else if (strncmp(token, "mport=", 5) == 0) {
				mport = atoi(token + 6);
			} else if (strncmp(token, "uport=", 5) == 0) {
				uport = atoi((token + 6));
			} else if (strncmp(token, "mip=", 3) == 0) {
				mipaddress = (token + 4);
			} else if (strncmp(token, "uip=", 3) == 0) {
				uipaddress = (token + 4);
			} else if (strncmp(token, "scale=", 5) == 0) {
				scale = strtod(token + 6, NULL);
			} else if (strncmp(token, "accellerometer.x=", 17) == 0) {
				acc.x = strtod(token + 17, NULL);
			} else if (strncmp(token, "accellerometer.y=", 17) == 0) {
				acc.y = strtod(token + 17, NULL);
			} else if (strncmp(token, "accellerometer.z=", 17) == 0) {
				acc.z = strtod(token + 17, NULL);
			} else if (strncmp(token, "magnetometer.x=", 15) == 0) {
				mag.x = strtod(token + 15, NULL);
			} else if (strncmp(token, "magnetometer.y=", 15) == 0) {
				mag.y = strtod(token + 15, NULL);
			} else if (strncmp(token, "magnetometer.z=", 15) == 0) {
				mag.z = strtod(token + 15, NULL);
			} else if (strncmp(token, "acc_min.x=", 10) == 0) {

				acc_min.x = strtod(token + 10, NULL);
			} else if (strncmp(token, "acc_min.y=", 10) == 0) {
				acc_min.y = strtod(token + 10, NULL);
			} else if (strncmp(token, "acc_min.z=", 10) == 0) {
				acc_min.z = strtod(token + 10, NULL);
			} else if (strncmp(token, "acc_max.x=", 10) == 0) {
				acc_max.x = strtod(token + 10, NULL);
			} else if (strncmp(token, "acc_max.y=", 10) == 0) {
				acc_max.y = strtod(token + 10, NULL);
			} else if (strncmp(token, "acc_max.z=", 10) == 0) {
				acc_max.z = strtod(token + 10, NULL);
			} else if (strncmp(token, "mag_min.x=", 10) == 0) {
				mag_min.x = strtod(token + 10, NULL);
			} else if (strncmp(token, "mag_min.y=", 10) == 0) {
				mag_min.y = strtod(token + 10, NULL);
			} else if (strncmp(token, "mag_min.z=", 10) == 0) {
				mag_min.z = strtod(token + 10, NULL);
			} else if (strncmp(token, "mag_max.x=", 10) == 0) {
				mag_max.x = strtod(token + 10, NULL);
			} else if (strncmp(token, "mag_max.y=", 10) == 0) {
				mag_max.y = strtod(token + 10, NULL);
			} else if (strncmp(token, "mag_max.z=", 10) == 0) {
				mag_max.z = strtod(token + 10, NULL);
			}

		} while ((token = strtok(NULL, ",\n")) != NULL);
}

void writecontroldev(const char* message) {

	size_t count;
	count = fwrite(message, 1, strlen(message), controldevice);
	//count = fwrite("\n", 1, 1, controldevice);
	//printf("Wrote %u bytes. to controldevice\n", count);
	while (fflush(controldevice) != 0)
		;
	//closedevs();
	//opendevs();
}

// Signals handler

void catchQuit(int sig) {
	printf("Caught signal %d Shutting down.\n", sig);
	quit = true;
}

void help() {
	printf("vrcontrol [-c controldevice][-t trackingdevice]\n");
}

void parsearguments(int argc, char *argv[]) {
	int c;
	while ((c = getopt(argc, argv, "ht:c:")) != -1)
		switch (c) {
		case 'h':
			help();
			exit(0);
			break;
		case 't':
			dev_name = optarg;
			break;
		case 'c':
			cdev_name = optarg;
			break;
		case '?':
			help();
			exit(0);
		default:
			return;
		}

}

int main(int argc, char *argv[]) {

	if (argc > 1)
		parsearguments(argc, argv);

	opendevs();

	// Setup signal handler

	signal(SIGTERM, catchQuit);
	signal(SIGQUIT, catchQuit);
	signal(SIGABRT, catchQuit);
	signal(SIGINT, catchQuit);

	// curses init
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	raw();
	sleep(1);

	int send = 0;

	while (!quit) {
		readtrackingdev();
		readcontroldev();

		attron( A_REVERSE);
		if (!calmode) {
			mvprintw(
					0,
					0,
					"        - C-callibrate Q-quit W-write configuration R-read configuration        \n");
			mvprintw(0, 0, device_name.c_str());
		} else {
			mvprintw(
					0,
					0,
					"        - C-normal mode R-reset calibration S-save calibration Z-set zero Q-quit\n");
			mvprintw(0, 0, device_name.c_str());
		}
		attroff(A_REVERSE);

		key = getch();
		if (key != ERR && key != -1) {
			if (*keyname(key) == 'c') {
				if (!calmode) {
					writecontroldev("callibration=1");
					writecontroldev("readcallibration");
				} else
					writecontroldev("callibration=0");
			}
			if (calmode) {
				if (*keyname(key) == 's')
					writecontroldev("savecallibration");
				if (*keyname(key) == 'z')
					writecontroldev("setzero");
				if (*keyname(key) == 'r') {
					writecontroldev("reset");
				}

			} else {

			}
			if (*keyname(key) == 'q') {
				quit = 1;
			}
			char command[16];
			if (*keyname(key) == 'y') {
				sprintf(command, "invert_yaw=%d", !invert_yaw);
				writecontroldev(command);
			}
			if (*keyname(key) == 'p') {
				sprintf(command, "invert_pitch=%d", !invert_pitch);
				writecontroldev(command);
			}
			if (*keyname(key) == 'r') {
				sprintf(command, "invert_roll=%d", !invert_roll);
				writecontroldev(command);
			}
			if (*keyname(key) == '+') {
				sprintf(command, "scale=%f", scale + 0.1);
				writecontroldev(command);
			}
			if (*keyname(key) == '-') {
				sprintf(command, "scale=%f", scale - 0.1);
				writecontroldev(command);
			}
			if (*keyname(key) == 'r')
				writecontroldev("readconfiguration");
			if (*keyname(key) == 'w')
				writecontroldev("saveconfiguration");
			if (*keyname(key) == 'j') {
				sprintf(command, "joystick=%d", !joystick);
				writecontroldev(command);
			}
			if (*keyname(key) == 'm') {
				sprintf(command, "mouse=%d", !mouse);
				writecontroldev(command);
			}
			if (*keyname(key) == 'u') {
				sprintf(command, "udp=%d", !udp);
				writecontroldev(command);
			}
			if (*keyname(key) == 't') {
				sprintf(command, "multicast=%d", !multicast);
				writecontroldev(command);
			}

		}

		if (calmode) {
			mvprintw(10, 0, "accellerometer x:%f\ty:%f\tz:%f\n", acc.x, acc.y,
					acc.z);
			mvprintw(11, 0, "magnetometer   x:%f\ty:%f\tz:%f\n", mag.x, mag.y,
					mag.z);

			mvprintw(13, 0, "acc_min.x: %f\tacc_min.y: %f\tacc_min.z: %f",
					acc_min.x, acc_min.y, acc_min.z);
			mvprintw(14, 0, "acc_max.x: %f\tacc_max.y: %f\tacc_max.z: %f",
					acc_max.x, acc_max.y, acc_max.z);
			mvprintw(15, 0, "mag_min.x: %f\tmag_min.y: %f\tmag_min.z: %f",
					mag_min.x, mag_min.y, mag_min.z);
			mvprintw(16, 0, "mag_max.x: %f\tmag_max.y: %f\tmag_max.z: %f",
					mag_max.x, mag_max.y, mag_max.z);

			mvprintw(
					18,
					0,
					"During calibration make sure that the display of the device is displaying\nsomething.");
			mvprintw(21, 0,
					"Turn around the device until the min/max values don't change.");
			mvprintw(
					22,
					0,
					"Press S to save callibration, R to reset, Z to set zero, C to exit\ncallibration mode.");

			//basic filter
			//cout<<fabs(fabs(acc.x)-fabs(acc_old.x))<<"<"<<(acc_max.x - acc_min.x)/ratio<<endl;
		}

		mvprintw(2, 0, "Sensor readings:");
		mvprintw(4, 0, "yaw:   % f\n", trackingdata.yaw);
		mvprintw(5, 0, "pitch: % f\n", trackingdata.pitch);
		mvprintw(6, 0, "roll:  % f\n", trackingdata.roll);

		mvprintw(2, 20, "Configuration:");
		mvprintw(4, 20, "invert yaw/x:   %d\n", invert_yaw);
		mvprintw(5, 20, "invert pitch/y: %d\n", invert_pitch);
		mvprintw(6, 20, "invert roll/z:  %d\n", invert_roll);
		mvprintw(7, 20, "scale(+-):    %2.1f\n", scale);

		mvprintw(2, 40, "Outputs:");
		mvprintw(4, 40, "Joystick:  %d\n", joystick);
		mvprintw(5, 40, "Mouse:     %d\n", mouse);
		mvprintw(6, 40, "UDP:       %d\n", udp);
		mvprintw(7, 40, "multicast: %d\n", multicast);

		mvprintw(6, 53, "%s:%d\n", uipaddress.c_str(), uport);
		mvprintw(7, 53, "%s:%d\n", mipaddress.c_str(), mport);

		//highlight active chars
		attron(A_REVERSE);
		mvprintw(4, 27, "y");
		mvprintw(5, 27, "p");
		mvprintw(6, 27, "r");
		mvprintw(7, 26, "+-");
		mvprintw(4, 40, "j");
		mvprintw(5, 40, "m");
		mvprintw(6, 40, "U");
		mvprintw(7, 43, "t");
		attroff(A_REVERSE);
		mvprintw(8, 0, "");

		refresh();
		usleep(delay); // Don't max out CPU
	}

	//cleanup curses
	endwin();
	closedevs();

	return 0;
}
