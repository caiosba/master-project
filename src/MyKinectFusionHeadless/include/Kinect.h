#ifndef KINECT_H
#define KINECT_H

#include <pcl/io/openni_grabber.h>
#include <pcl/io/oni_grabber.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class Kinect
{
public:
	Kinect(bool live, char *onifile);
	~Kinect();
	bool grabFrame();
	boost::shared_ptr<openni_wrapper::Image>& getRGBImage() { return rgbImage; }
	boost::shared_ptr<openni_wrapper::DepthImage>& getDepthImage() { return depthImage; }
  boost::shared_ptr<openni_wrapper::DepthImage> removeBackground(const boost::shared_ptr<openni_wrapper::DepthImage>& depthImage, const boost::shared_ptr<openni_wrapper::Image>& rgbImage);
private:
	void imageCallBack (const boost::shared_ptr<openni_wrapper::Image>& rgbImage, const boost::shared_ptr<openni_wrapper::DepthImage>& depthImage, float constant);
	
	pcl::Grabber *capture;
	boost::condition_variable data_ready_cond;
	boost::mutex data_ready_mutex;
	boost::shared_ptr<openni_wrapper::Image> rgbImage;
	boost::shared_ptr<openni_wrapper::DepthImage> depthImage;
};

#endif
