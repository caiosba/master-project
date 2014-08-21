#ifndef SDF_H
#define SDF_H

#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/Cholesky>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <vector>
#include <pcl/pcl_macros.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/gpu/containers/device_array.h>
#include "../tools/openni_capture.h"
//#include "../pcl-trunk/gpu/kinfu/tools/openni_capture.h"
#include <pcl/gpu/kinfu/kinfu.h>
#include <pcl/gpu/containers/kernel_containers.h>
#include "internal.h"
#include <pcl/gpu/containers/initialization.h>
#include "MyPointCloud.h"

using namespace Eigen;
using namespace pcl;
using namespace pcl::gpu;
typedef short2 volume_elem_type;

class TsdfVolume
{
public:
	TsdfVolume(Eigen::Vector3i& volumeSize);

	void integrateVolume(std::vector<Matrix3frm>& rmats, std::vector<Vector3f>& tvecs, device::DepthMap &depthRaw, device::Intr& intrinsics,
	float trancDist, DeviceArray2D<float>& depthRawScaled, int globalTime);
	void raycast(std::vector<Matrix3frm>& rmats, std::vector<Vector3f>& tvecs, device::Intr& intrinsics, float trancDist, MyPointCloud *globalPreviousPointCloud, int globalTime);
	void raycastFromPose(std::vector<Matrix3frm>& rmats, std::vector<Vector3f>& tvecs, device::Intr& intrinsics, float trancDist, MyPointCloud *globalPreviousPointCloud, int globalTime, Matrix3frm pose_rmats, Vector3f pose_tvecs);
	DeviceArray<PointXYZ> fetchCloud(DeviceArray<PointXYZ>& cloud_buffer);

	//10 * 1000 * 1000
	enum { DEFAULT_CLOUD_BUFFER_SIZE = 10 * 1000 * 1000 };

	Eigen::Vector3f& getVolumeSize() { return volumeSize_; }
	void getHostErrorInRGB(KinfuTracker::View &errorInRGB);
	void getHostErrorInRGB();
	DeviceArray2D<int> getVolume() { return volume_; }

	void setTSDFVisualization(bool hasTSDFVisualization) { hasTSDFVisualization_ = hasTSDFVisualization; }
	void reset() {  device::initVolume<volume_elem_type> (volume_); }
        void transformCamera(std::vector<Matrix3frm>& Rcam, std::vector<Vector3f>& tcam, int globalTime);
	
private:
	Eigen::Vector3f volumeSize_;
	DeviceArray2D<int> volume_;
	DeviceArray2D<float> error_;
	bool hasTSDFVisualization_;
};

#endif
