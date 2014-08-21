#include "Kinect.h"

Kinect::Kinect(bool live, char *onifile)
{
  try {
		if (live) {
	    capture = new pcl::OpenNIGrabber("#2");
		}
	  
    else {
      bool triggered_capture = false;
      bool repeat = true;
      capture = new pcl::ONIGrabber(onifile, repeat, !triggered_capture);
	  }

	  boost::function<void (const boost::shared_ptr<openni_wrapper::Image>&, const boost::shared_ptr<openni_wrapper::DepthImage>&, float constant)> callbackFunction
	  	= boost::bind (&Kinect::imageCallBack, this, _1, _2, _3);
	  capture->registerCallback(callbackFunction);
	  capture->start();
  }
	catch (const pcl::PCLException&) { std::cout << "Can't open depth source" << std::endl, -1; }

}

Kinect::~Kinect()
{
	capture->stop();
	delete capture;
}

bool Kinect::grabFrame()
{
	boost::unique_lock<boost::mutex> lock(data_ready_mutex);
	return data_ready_cond.timed_wait (lock, boost::posix_time::millisec(100));
}

void Kinect::imageCallBack (const boost::shared_ptr<openni_wrapper::Image>& rgbImage, const boost::shared_ptr<openni_wrapper::DepthImage>& depthImage, float constant) {

	boost::mutex::scoped_try_lock lock(data_ready_mutex);

	if (!lock)
		return;

	this->rgbImage = rgbImage;
	// this->depthImage = removeBackground(depthImage, rgbImage);
	this->depthImage = depthImage;

	data_ready_cond.notify_one();

}

boost::shared_ptr<openni_wrapper::DepthImage> Kinect::removeBackground(const boost::shared_ptr<openni_wrapper::DepthImage>& depthImage, const boost::shared_ptr<openni_wrapper::Image>& rgbImage)
{
	unsigned short *depthData = (unsigned short*)malloc(640 * 480 * sizeof(unsigned short));
	IplImage *rgbData = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	int w = this->rgbImage->getWidth();
	int h = this->rgbImage->getHeight();
	
	// Convert the data so we can manipulate it
	depthImage->fillDepthImageRaw(w, h, depthData);
	rgbImage->fillRGB(w, h, (unsigned char *)rgbData->imageData);

	IplImage *grayImage = cvCreateImage(cvGetSize(rgbData), IPL_DEPTH_8U, 1);
	cvCvtColor(rgbData, grayImage, CV_RGB2GRAY);
	IplImage *bwImage = cvCreateImage(cvGetSize(grayImage),IPL_DEPTH_8U, 1);
	// FIXME: Find a better threshold than 96
	cvThreshold(grayImage, bwImage, 96, 255, CV_THRESH_BINARY);
	unsigned char *bwData = reinterpret_cast<unsigned char *>(bwImage->imageData);
	// cvSaveImage("/tmp/image_bw.png", bwImage);
	
	// We update the depth map based on the binarized image (black and white)
	for (int pixel = 0; pixel < w * h; pixel++)
		if (bwData[pixel] == 0)
			depthData[pixel] = 0;

	// Convert back
	boost::shared_ptr<xn::DepthMetaData> depthMetaData (new xn::DepthMetaData);
	depthMetaData->AllocateData(w, h);
	XnDepthPixel* depthMap = depthMetaData->WritableData();
	for(int pixel = 0; pixel < w * h; pixel++) {
		depthMap[pixel] = static_cast<XnDepthPixel>(depthData[pixel]);
	}
	boost::shared_ptr<openni_wrapper::DepthImage> depthImageNew (new openni_wrapper::DepthImage(depthMetaData, 0.075f, 525.f, 0, 0));
	
	// Free memory
	cvReleaseImage(&rgbData);
	cvReleaseImage(&grayImage);
	cvReleaseImage(&bwImage);
	
	return depthImageNew;
}
