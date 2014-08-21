#ifndef COLORED_RECONSTRUCTION_MEDIATOR_H
#define COLORED_RECONSTRUCTION_MEDIATOR_H

#include "Reconstruction.h"
#include "ColorVolume.h"
#include "TsdfVolume.h"
#include <pcl/io/pcd_io.h>

class ColoredReconstructionMediator
{
public:
	ColoredReconstructionMediator(Eigen::Vector3f& volumeSize);
	void savePointCloud(TsdfVolume* tsdfVolume);
	void updateColorVolume(Reconstruction *reconstruction);
private:
	ColorVolume* colorVolume;
	
};

#endif
