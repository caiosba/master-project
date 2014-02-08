// g++ freenectcv.cpp -o freenectcv -L/usr/local/lib/freenect -lfreenect `pkg-config opencv --cflags` `pkg-config opencv --libs` -I/usr/include/libusb-1.0 -I/usr/local/include/libfreenect
// Stereo-calibrates Kinect and another camera

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libfreenect.h>
#include <pthread.h>
#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
 
#define FREENECTOPENCV_RGB_DEPTH 3
#define FREENECTOPENCV_RGB_WIDTH 640
#define FREENECTOPENCV_RGB_HEIGHT 480
#define FREENECT_FRAME_W 640 
#define FREENECT_FRAME_H 480 
#define FREENECT_FRAME_PIX (FREENECT_FRAME_H*FREENECT_FRAME_W) // width*height pixels in the image 
#define FREENECT_VIDEO_RGB_SIZE (FREENECT_FRAME_PIX*3) //3 bytes per pixel 
 
using namespace cv;
using namespace std;
 
Mat rgbimg, tempimg;
 
pthread_mutex_t mutex_rgb = PTHREAD_MUTEX_INITIALIZER;
pthread_t cv_thread;
 
// callback for rgbimage, called by libfreenect
 
void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
 
  // lock mutex for opencv rgb image
  pthread_mutex_lock( &mutex_rgb );
  memcpy(rgbimg.data, rgb, FREENECT_VIDEO_RGB_SIZE);
  // unlock mutex
  pthread_mutex_unlock( &mutex_rgb );
}
 
/*
 * thread for displaying the opencv content
 */
void *cv_threadfunc (void *ptr) {
  rgbimg = Mat(FREENECTOPENCV_RGB_HEIGHT, FREENECTOPENCV_RGB_WIDTH, CV_8UC3);
  tempimg = Mat(FREENECTOPENCV_RGB_HEIGHT, FREENECTOPENCV_RGB_WIDTH, CV_8UC3);

  int numBoards = 30;
  int board_w = 8;
  int board_h = 5;

  Size board_sz = Size(board_w, board_h);
  int board_n = board_w*board_h;

  vector<vector<Point3f> > object_points;
  vector<vector<Point2f> > imagePoints1, imagePoints2;
  vector<Point2f> corners1, corners2;

  vector<Point3f> obj;
  for (int j=0; j<board_n; j++)
  {
      obj.push_back(Point3f(j/board_w, j%board_w, 0.0f));
  }

  Mat img1, img2, gray1, gray2;
  VideoCapture cap1 = VideoCapture(1);

  int success = 0, k = 0;
  int corner_count;
  bool found1 = false, found2 = false;
 
  // while start
  while (success < numBoards)
  {

    // Get image from Kinect
    pthread_mutex_lock( &mutex_rgb );
    cvtColor(rgbimg, img1, CV_BGR2RGB);
    cvtColor(img1, gray1, CV_BGR2GRAY);
    // imshow(FREENECTOPENCV_WINDOW_N, img2);
    pthread_mutex_unlock( &mutex_rgb );

    // Get image from camera
    cap1 >> img2;
    cvtColor(img2, gray2, CV_BGR2GRAY);

		waitKey(10);
    
    // FIXME: grayx or imgx?
    found1 = findChessboardCorners(gray1, board_sz, corners1, CV_CALIB_CB_ADAPTIVE_THRESH); // | CV_CALIB_CB_FILTER_QUADS);
    found2 = findChessboardCorners(gray2, board_sz, corners2, CV_CALIB_CB_ADAPTIVE_THRESH); // | CV_CALIB_CB_FILTER_QUADS);

    if (found1)
    {
        cornerSubPix(gray1, corners1, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(gray1, board_sz, corners1, found1);
        printf("Found a chessboard on image 1\n");
    } else printf("Found nothing for image 1\n");

    if (found2)
    {
        cornerSubPix(gray2, corners2, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(gray2, board_sz, corners2, found2);
        printf("Found a chessboard on image 2\n");
    } else printf("Found nothing for image 2\n");
    
    imshow("image1", gray1);
    imshow("image2", gray2);

    k = waitKey(30);
    printf("Key with code %d was pressed\n", k);
    if (found1 && found2)
    {
        k = waitKey(0);
    }
    if (k == 27)
    {
        break;
    }
    if ((char)k == 's' && found1 !=0 && found2 != 0)
    {
        imagePoints1.push_back(corners1);
        imagePoints2.push_back(corners2);
        object_points.push_back(obj);
        success++;
        printf ("Corners stored (%d)\n", success);

        if (success >= numBoards)
        {
				    printf("Got %d boards\n", success);
            break;
        }
    }
 
    // wait for quit key
    if(cvWaitKey(15) == 27)
      break;
 
  }
  // while end
  printf("Chessboard capturing end\n");

  destroyAllWindows();
  printf("Starting Calibration\n");
  Mat CM1 = Mat(3, 3, CV_64FC1); // Camera matrix
  Mat CM2 = Mat(3, 3, CV_64FC1); // Camera matrix
  Mat D1, D2; // Intrinsics (distance coefficients)
  Mat R, T, E, F;
	std::vector<Mat> r1, t1, r2, t2;

  calibrateCamera(object_points, imagePoints1, cvSize(640, 480), CM1, D1, r1, t1, 0);
  calibrateCamera(object_points, imagePoints2, cvSize(640, 480), CM2, D2, r2, t2, 0);

  printf("Calibrated camera 1\nCM1:\n");
	cout << CM1 << endl;
	printf("D1:\n");
	cout << D1 << endl;
  printf("Calibrated camera 2\nCM2:\n");
	cout << CM2 << endl;
	printf("D2:\n");
	cout << D2 << endl;

  stereoCalibrate(object_points, imagePoints1, imagePoints2, 
                  CM1, D1, CM2, D2, img1.size(), R, T, E, F,
                  cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5), 
                  CV_CALIB_FIX_INTRINSIC + CV_CALIB_ZERO_TANGENT_DIST + CV_CALIB_SAME_FOCAL_LENGTH);

  FileStorage fs1("mystereocalib.yml", FileStorage::WRITE);
  // fs1 << "CM1" << CM1;
  // fs1 << "CM2" << CM2;
  // fs1 << "D1" << D1;
  // fs1 << "D2" << D2;
  fs1 << "R" << R;
  fs1 << "T" << T;
  // fs1 << "E" << E;
  // fs1 << "F" << F;

  printf("Done Calibration\n");

  printf("Starting Rectification\n");

  Mat R1, R2, P1, P2, Q;
  stereoRectify(CM1, D1, CM2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);
	/*
  fs1 << "R1" << R1;
  fs1 << "R2" << R2;
  fs1 << "P1" << P1;
  fs1 << "P2" << P2;
  fs1 << "Q" << Q;
	*/
	fs1.release();

  printf("Done Rectification\n");

  printf("Applying Undistort\n");

  Mat map1x, map1y, map2x, map2y;
  Mat imgU1, imgU2;

  initUndistortRectifyMap(CM1, D1, R1, P1, img1.size(), CV_32FC1, map1x, map1y);
  initUndistortRectifyMap(CM2, D2, R2, P2, img2.size(), CV_32FC1, map2x, map2y);

  printf("Undistort complete\n");

  while(1)
  {    
      cap1 >> img1;
      pthread_mutex_lock(&mutex_rgb);
      cvtColor(rgbimg,img2,CV_BGR2RGB);
      pthread_mutex_unlock(&mutex_rgb);

      remap(img1, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
      remap(img2, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

      imshow("image1", imgU1);
      imshow("image2", imgU2);

      k = waitKey(10);

      if (k == 27)
      {
          break;
      }
  }

  cap1.release();

  // Finish
  pthread_exit(NULL); 
  return NULL;
}
 
int main(int argc, char **argv)
{
 
  freenect_context *f_ctx;
  freenect_device *f_dev;
 
  int res = 0;
  int die = 0;
  printf("Kinect camera test\n");
 
  if (freenect_init(&f_ctx, NULL) < 0)
  {
    printf("freenect_init() failed\n");
    return 1;
  }
 
  if (freenect_open_device(f_ctx, &f_dev, 0) < 0)
  {
    printf("Could not open device\n");
    return 1;
  }
 
  freenect_set_video_callback(f_dev, rgb_cb);
  freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
 
  // create opencv display thread
  res = pthread_create(&cv_thread, NULL, cv_threadfunc, NULL);
  if (res)
  {
    printf("pthread_create failed\n");
    return 1;
  }
  printf("init done\n");
 
  freenect_start_video(f_dev);
 
  while(!die && freenect_process_events(f_ctx) >= 0 );
}
