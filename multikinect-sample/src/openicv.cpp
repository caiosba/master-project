/**
 * openicv.cpp
 * Author: heekinho
 * Date: 11/03/2013
 *
 * Based on a code found in:
 * http://blog.csdn.net/wobuaishangdiao/article/details/8489114
 */

#include "iostream"
#include "OpenNI.h"

#include "opencv2/opencv.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "pandaViewer.h"

using namespace std;
using namespace openni;
using namespace cv;

int numframe = 0;

Device device;
VideoStream depth, image;
VideoStream* streams[2];
VideoFrameRef frame;


int readKinectFrame(void);


void show_chessboard(Mat& image){
    // Output vectors of image points
    std::vector<cv::Point2f> imageCorners;

    // Number of corners on the chessboard
    cv::Size boardSize(9, 6);

    // Get the chessboard corners
    bool found = cv::findChessboardCorners(image, boardSize, imageCorners, 2 & 4);

    // Draw the corners
    cv::drawChessboardCorners(image, boardSize, imageCorners, found);

    const char* chessboardstr = "chessboard";
    namedWindow(chessboardstr);
    imshow(chessboardstr, image);
}


/**
 * TODO: Verificar se utilizarmos o depth map para obter a posição (distância) e rotação,
 * ao invés de utilizar apenas o rgb e chessboard ajuda no processo de calibração.
 */
void findMatrix(vector<Point3f> &objectPoints, vector<Point2f> &imagePoints, Mat intrinsic, Mat distCoeffs){
    Mat rvec, tvec;
    solvePnP(objectPoints, imagePoints, intrinsic, distCoeffs, rvec, tvec);
//    cout << "Rotation: " << rvec << endl;
//    cout << "Translation: " << tvec << endl;


    // Calcula a matriz de rotação usando Rodrigues
    Mat rotMat = Mat(3, 3, CV_64F);
    Rodrigues(rvec, rotMat);
//    cout << rotMat << endl;

    // Calcula a transposta da matriz para fazer a homogenea
    Mat rotMatT = rotMat.t();
//    cout << rotMatT << endl;

    Mat RTt = -rotMatT * tvec;
//    cout << "RTt: " << RTt << endl;

    // Obtem a matriz homogenea
    Mat homogenea = Mat::eye(4, 4, CV_64F);

    // Colocando a matriz de rotação transposta na homogenea
    for(int i = 0; i < 3; i++){
    	for(int j = 0; j < 3; j++){
    		homogenea.at<double>(i, j) = rotMatT.at<double>(i, j);
    	}
    }

    // Colocando a translação na homogenea
    for(int i = 0; i < 3; i++){
    	homogenea.at<double>(i, 3) = RTt.at<double>(0, i);
    }

    cout << "Matrix: " <<  homogenea << endl;

    set_mat(homogenea, numframe);
}

///*!
// * Average corners to hopefully get a better calibration
// */
//void average_corners(const vector<Point2f> &corners, vector<Point2f> &avg_corners){
//	if(avg_corners.size() == 0){
//		for(int i = 0; i < corners.size(); i++){
//			avg_corners.push_back(corners[i]);
//		}
//	}
//	else {
//		for(int i = 0; i < corners.size(); i++){
//			avg_corners[i] = ((avg_corners[i] + corners[i]) * 0.5f);
//		}
//	}
//}


/*!
 * Ao invés de depth, obtemos aqui uma imagem pronta para ser passado
 * para o shader e visualizarmos. Evita o cálculo no shader, mas é mais
 * pesado.
 *
 * NOTA: Implementei isso aqui em shader (depth.cg)
 */
void get3DMapping(){
//	DepthPixel depthValue = depth.getMaxPixelValue();
//	float x, y, z;
//	CoordinateConverter::convertDepthToWorld(depth, 300, 300, depthValue, &x, &y, &z);

	cout << "resX: " << frame.getVideoMode().getResolutionX() << endl;
	cout << "resX: " << frame.getVideoMode().getResolutionY() << endl;

	cout << "fovH: " << depth.getHorizontalFieldOfView() << endl;
	cout << "fovV: " << depth.getVerticalFieldOfView() << endl;

	cout << "xzFactor: " << tan(depth.getHorizontalFieldOfView() / 2.0) * 2.0 << endl;
	cout << "xyFactor: " << tan(depth.getVerticalFieldOfView() / 2.0) * 2.0 << endl;
}

/*!
 * Procedimento de calibração da camera do kinect.
 * Obtenção da matriz intrínseca, necessária posteriormente.
 */
void kinectRgbCalibration(int numShots, int numCornersHor, int numCornersVer){
	int numSquares = numCornersHor * numCornersVer;
	Size boardSize = Size(numCornersHor, numCornersVer);

    // Physical position of the corners (3D space).
    vector< vector<Point3f> > objectPoints;

    // Physical position of corners in the image (2D space)
    vector< vector<Point2f> > imagePoints;


    /* This creates a list of coordinates (0,0,0), (0,1,0), (0,2,0)…(1,4,0)…
     * so on. Each corresponds to a particular vertex.
     * Suppose the squares in your chessboards were 30mm in size,
     * and you supplied these coordinates as (0,0,0), (0, 30, 0)
     */
    vector<Point3f> boardReference;
    for(int i = 0; i < numSquares; i++){
        // We're not really concerned with the physical dimensions
        boardReference.push_back(Point3f(i/numCornersHor, i%numCornersHor, 0.0f));
    }

    // Armazena cada canto detectado pelo findChessboardCorners
    vector<Point2f> corners;

    Mat image, grayImage;
    int successes = 0;
    char key = 0;

    // Realiza captura dos cantos do chessboards para posterior calibração
    while (key != 27 && successes < numShots) {
    	//cout << "Reading frame..." << endl;
    	readKinectFrame();

    	// Só processamos quando o frame for RGB
    	if(frame.getVideoMode().getPixelFormat() == PIXEL_FORMAT_RGB888){
    		image = Mat(frame.getHeight(), frame.getWidth(), CV_8UC3, (void*)frame.getData());
    		cvtColor(image, image, CV_BGR2RGB);
    		cvtColor(image, grayImage, CV_RGB2GRAY);

    		bool found = findChessboardCorners(grayImage, boardSize, corners,  (CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS));
    		if(found){
				cornerSubPix(grayImage, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
				drawChessboardCorners(image, boardSize, corners, found);
    		}

    		imshow("RGB Kinect", image);
    		//imshow("Gray Image", grayImage);

    		key = cvWaitKey(10);

			// Space was pressed and chessboard pattern found
			if (key == ' ' && found != 0) {
				imagePoints.push_back(corners);
				objectPoints.push_back(boardReference);

				cout << "Board Captured!" << endl;
				successes++;
			}
    	}
    }

    // Declaring our unknown parameters
    Mat intrinsic = Mat(3, 3, CV_64FC1);
    Mat distCoeffs;
    vector<Mat> rVecs;
    vector<Mat> tVecs;

    // Focal lenghts along X and Y axis
    intrinsic.ptr(0)[0] = 1;
    intrinsic.ptr(1)[1] = 1;

    // Finally, the calibration
    calibrateCamera(objectPoints, imagePoints, image.size(), intrinsic, distCoeffs, rVecs, tVecs);

    cout << "Intrinsic: " << intrinsic << endl;
    cout << "Distortion Coefficients: " << distCoeffs << endl;
}


void trackCameraPosition(int numCornersHor, int numCornersVer){
    // Hardcoded aqui depois da calibração, mas enfim...
	Mat intrinsic = (Mat_<double>(3,3) << 526.0, 0, 318.0, 0, 526.0, 270.0, 0, 0, 1);
	Mat distCoeffs = (Mat_<double>(1,5) << 0.2396517942750204, -0.9133942441833466, 0.000202332030289716, 0.001075496181332869, 1.275027377722337);

	int numSquares = numCornersHor * numCornersVer;
	Size boardSize = Size(numCornersHor, numCornersVer);

    vector<Point2f> corners;
    vector<Point3f> boardReference;
    for(int i = 0; i < numSquares; i++){
        boardReference.push_back(Point3f(i/numCornersHor, i%numCornersHor, 0.0f));
    }

    Mat imageDistorted, image, grayImage, imageDepth;
    char key = 0;

    while (key != 27) {
    	//cout << "Reading frame..." << endl;
    	readKinectFrame();

    	// Só processamos quando o frame for RGB
    	if(frame.getVideoMode().getPixelFormat() == PIXEL_FORMAT_RGB888){
//    		imageDistorted = Mat(frame.getHeight(), frame.getWidth(), CV_8UC3, (void*)frame.getData());
//    		undistort(imageDistorted, image, intrinsic, distCoeffs);
    		image = Mat(frame.getHeight(), frame.getWidth(), CV_8UC3, (void*)frame.getData());
    		cvtColor(image, image, CV_BGR2RGB);
    		cvtColor(image, grayImage, CV_RGB2GRAY);

			set_rgb_data(image.cols, image.rows, (void*)image.data, numframe);

    		bool found = findChessboardCorners(grayImage, boardSize, corners,  (CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS));
    		if(found){
				cornerSubPix(grayImage, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
				drawChessboardCorners(image, boardSize, corners, found);

				findMatrix(boardReference, corners, intrinsic, distCoeffs);
    		}

    		imshow("RGB Kinect", image);
    		//imshow("Gray Image", grayImage);

    	}
    	else if((frame.getVideoMode().getPixelFormat() == PIXEL_FORMAT_DEPTH_100_UM) ||
    			(frame.getVideoMode().getPixelFormat() == PIXEL_FORMAT_DEPTH_1_MM) ){
//    		imageDistorted = Mat(frame.getHeight(), frame.getWidth(), CV_16UC1, (void*)frame.getData());
//    		undistort(imageDistorted, imageDepth, intrinsic, distCoeffs);
    		imageDepth = Mat(frame.getHeight(), frame.getWidth(), CV_16UC1, (void*)frame.getData());

    		Mat imgDEP(frame.getHeight(), frame.getWidth(), CV_16UC1, (void*) frame.getData());

//    		// Converte para mostrar
//    		imgDEP.convertTo(imgDEP, CV_8U, 255.0 / 4096.0);
//    		imshow("Image Depth", imgDEP);

    		//get3DMapping();

    		set_depth_data(imageDepth.cols, imageDepth.rows, (void*)imageDepth.data, numframe);
    	}

		key = cvWaitKey(10);
		if(key == 'a'){
			cout << "Space" << endl;
			numframe++;
		}
    }
}


Status initOpenNI(){
	// Initialize OpenNI
	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK) {
		cout << "Initialize failed" << OpenNI::getExtendedError();
		cin.get();
		cin.get();
		return rc;
	}

	// Open Device
	rc = device.open(ANY_DEVICE);

	// Get Depth Sensor
	if (device.getSensorInfo(SENSOR_DEPTH) != NULL) {
		rc = depth.create(device, SENSOR_DEPTH);

		if (rc == STATUS_OK) {
			rc = depth.start();
			if (rc != STATUS_OK) {
				cout << "couldn't create depth stream" << OpenNI::getExtendedError();
				cin.get();
				cin.get();
				return rc;
			}
		}
	}

	// Get Color Sensor
	if (device.getSensorInfo(SENSOR_COLOR) != NULL) {
		rc = image.create(device, SENSOR_COLOR);

		if (rc == STATUS_OK) {
			rc = image.start();
		}
	}

	streams[0] = &depth;
	streams[1] = &image;

	if (device.hasSensor(SENSOR_COLOR) && device.hasSensor(SENSOR_DEPTH)) cout << "The color is OK" << endl;;

	return rc;
}

int readKinectFrame(){
	int readyStream = -1;
	//for the readFrame, you should waitForAnyStream to get the new frame;
	Status rc = OpenNI::waitForAnyStream(streams, 2, &readyStream);
	switch (readyStream) {
		case 0:
			//depth
			depth.readFrame(&frame);
			break;
		case 1:
			//color
			image.readFrame(&frame);
			break;
		default:
			cout << "unexpected stream" << endl;
			break;
	}

	return readyStream;
}


int main(int argc, char** argv) {
	/* Create a panda thread for visualization */
	pthread_t thread_id;
	int param;
	pthread_create(&thread_id, NULL, panda_thread, &param);

	// Initialize OpenNI
	Status rc = initOpenNI();
	if(rc != STATUS_OK){
		cout << "Error while loading OpenNI. Exiting..." << endl;
		return 1;
	}

//	kinectRgbCalibration(10, 9, 6);
	trackCameraPosition(9, 6);
//	trackCameraPositionFromBalls();


	image.stop();
	image.destroy();
	depth.stop();
	depth.destroy();
	device.close();
	OpenNI::shutdown();

	cin.get();
	cin.get();
	return 0;
}
