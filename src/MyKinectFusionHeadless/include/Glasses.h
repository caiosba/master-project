#ifndef GLASSES_H
#define GLASSES_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <ctype.h>

#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
using namespace std;

const int MAX_COUNT = 500;

class Glasses
{
public:
  Glasses(int port_number);
	~Glasses();
	void get();
	void getYawPitchRoll();
	void getXYZ();
	void init();
	void initYawPitchRoll();
	void initXYZ();
	void finish();
	void finishYawPitchRoll();
	void finishXYZ();
	void zero();
	void zeroXYZ();
	void zeroYawPitchRoll();
	long getYaw() { return yaw; }
	long getPitch() { return pitch; }
	long getRoll() { return roll; }
	double getX() { return x - xi; }
	double getY() { return y - yi; }
	double getZ() { return z; }
	Mat getFrame() { return frame; }

private:
  int sock;
	int port;
  bool addRemovePt;
	bool useSimulatedData;
  Mat gray;
	Mat prevGray;
	Mat image;
	Mat homo;
	Mat frame;
  vector<Point2f> points[2];
  Point2f point;
  VideoCapture cap;
  long yaw;
  long pitch;
  long roll;
  double x;
  double y;
  double z;
	double xi;
	double yi;
	bool initialized;
};

#endif
