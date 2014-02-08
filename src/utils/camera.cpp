// g++ camera.cpp -lopencv_core -lopencv_video -lopencv_highgui -lopencv_flann -lopencv_objdetect -lopencv_imgproc -lopencv_calib3d -o camera
// Show image from camera 

#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
  while (true) {
    Mat img, gray;
    VideoCapture cap = VideoCapture(1); // Change ID in order to change camera
    cap >> img;
    resize(img, img, Size(640, 480));
    cvtColor(img, gray, CV_BGR2GRAY);
    imshow("Image from camera", gray);
    waitKey(10);
  }
}
