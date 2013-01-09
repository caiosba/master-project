#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <curses.h>

struct TrackingData {
	double viewmatrix[16];
	double yaw;
	double pitch;
	double roll;
};

TrackingData trackingdata;

struct ip_mreq command;
int socket_descriptor;
socklen_t sin_len;
struct sockaddr_in sin;

char* multicastAddress = "224.0.0.42";
int port = 4242;

bool connected = false;
bool quit = false;
char key;

void help() {
	std::cout
			<< "democlient [-h] [--help] [-m multicast ip] [-p multicast port]"
			<< std::endl;
}

void catchQuit(int sig) {
	printf("Caught signal %d Shutting down.\n", sig);
	quit = true;
}

void parsearguments(int argc, char *argv[]) {
	int c;
	while ((c = getopt(argc, argv, "hm:p:c:")) != -1)
		switch (c) {
		case 'h':
			help();
			exit(0);
			break;
		case 'm':
			multicastAddress = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case '?':
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			exit(0);
		default:
			return;
		}

}

int createMulticastSocket() {
	int loop = 1;
	struct sockaddr_in sin;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	if ((socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	/* Allow multiple processes to use the same port */
	loop = 1;
	if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &loop,
			sizeof(loop)) < 0) {
		perror("setsockopt:SO_REUSEADDR");
		exit(EXIT_FAILURE);
	}

	if (bind(socket_descriptor, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	/* allow multicast */
	loop = 1;
	if (setsockopt(socket_descriptor, IPPROTO_IP, IP_MULTICAST_LOOP, &loop,
			sizeof(loop)) < 0) {
		printf("setsockopt:IP_MULTICAST_LOOP");
		exit(EXIT_FAILURE);
	}
	/* join the multicast group: */
	command.imr_multiaddr.s_addr = inet_addr(multicastAddress);
	command.imr_interface.s_addr = htonl(INADDR_ANY);
	if (command.imr_multiaddr.s_addr == -1) {
		printf("%s is no multicast address\n", multicastAddress);
		exit(EXIT_FAILURE);
	}
	if (setsockopt(socket_descriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, &command,
			sizeof(command)) < 0) {
		printf("setsockopt:IP_ADD_MEMBERSHIP");
	}

	fcntl(socket_descriptor, F_SETFL, O_NONBLOCK);

	return socket_descriptor;
}

int main(int argc, char *argv[]) {
	if (argc > 1)
		parsearguments(argc, argv);

	// Setup signal handler

	signal(SIGTERM, catchQuit);
	signal(SIGQUIT, catchQuit);
	signal(SIGABRT, catchQuit);
	signal(SIGINT, catchQuit);

	std::cout << "receiving multicasts: " << multicastAddress << ":" << port
			<< std::endl;

	// curses init
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	raw();
	sleep(1);

	while (!quit) {
		attron( A_REVERSE);
		mvprintw(
				0,
				0,
				"Demo client for VR920 Headtracking driver - Press Q to exit.                  \n");
		attroff(A_REVERSE);

		if (!connected) {
			if (createMulticastSocket() == -1) {
				std::cerr << "Could not connect to daemon" << std::endl;
				break;
			}

			connected = true;
		}

		key = getch();
		if (key != ERR && key != -1) {
			if (*keyname(key) == 'q')
				quit = 1;
		}

		if (recvfrom(socket_descriptor, &trackingdata, sizeof(TrackingData), 0,
				(struct sockaddr *) &sin, &sin_len) != -1) {
			mvprintw(3, 0, "Angles:");
			mvprintw(5, 0, "yaw: %f\n", trackingdata.yaw);
			mvprintw(6, 0, "acc_roll: %f\n", trackingdata.roll);
			mvprintw(7, 0, "acc_pitch: %f\n", trackingdata.pitch);

			mvprintw(12, 0, "ViewMatrix:");
			mvprintw(14, 0, "%f\t%f\t%f\t%f", trackingdata.viewmatrix[0],
					trackingdata.viewmatrix[1], trackingdata.viewmatrix[2],
					trackingdata.viewmatrix[3]);
			mvprintw(15, 0, "%f\t%f\t%f\t%f", trackingdata.viewmatrix[4],
					trackingdata.viewmatrix[5], trackingdata.viewmatrix[6],
					trackingdata.viewmatrix[7]);
			mvprintw(16, 0, "%f\t%f\t%f\t%f", trackingdata.viewmatrix[8],
					trackingdata.viewmatrix[9], trackingdata.viewmatrix[10],
					trackingdata.viewmatrix[11]);
			mvprintw(17, 0, "%f\t%f\t%f\t%f", trackingdata.viewmatrix[12],
					trackingdata.viewmatrix[13], trackingdata.viewmatrix[14],
					trackingdata.viewmatrix[15]);
		}
		refresh();
		usleep(1000);
	}

	//cleanup socket
	if (setsockopt(socket_descriptor, IPPROTO_IP, IP_DROP_MEMBERSHIP, &command,
			sizeof(command)) < 0) {
		perror("setsockopt:IP_DROP_MEMBERSHIP");
	}
	close(socket_descriptor);

	//cleanup curses
	endwin();
	std::cout << "end";
}

