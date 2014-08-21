#!/bin/bash
# Install MyKinectFusion on Ubuntu 12.04
# Please run as root

# All PCL except for GPU
add-apt-repository ppa:v-launchpad-jochen-sprickerhof-de/pcl
apt-get update

# Some utilities that your computer must have
apt-get install git-core unzip cmake-curses-gui g++ libcudart4 nvidia-cuda-dev nvidia-cuda-doc nvidia-cuda-gdb nvidia-cuda-toolkit python-pycuda python-pycuda-doc python-pycuda-headers libpcl-all libglew1.6-dev libglew1.6 freeglut3-dev freeglut3 libopencv-core-dev libopencv-core2.3 libopencv-contrib2.3 libopencv-contrib-dev libopencv-dev libopencv-flann2.3 libopencv-flann-dev libopencv-highgui-dev libopencv-highgui2.3 libopencv-imgproc2.3 libopencv-imgproc-dev cmake-curses-gui libhighgui-dev libcv-dev

# If Kinect is not found, use drivers from ps-engine package:
apt-get install ps-engine

# Use custom opencv2 version (Debian Squeeze version, works on Ubuntu 12.04)
rm -rf /usr/local/include/opencv2
cp -r lib/opencv2 /usr/local/include/

# PCL from trunk, due to GPU support, which is not packed
# You can try `git clone`, but sometimes it can be stuck due to the repository size
# git clone https://github.com/PointCloudLibrary/pcl pcl-trunk
wget https://github.com/PointCloudLibrary/pcl/archive/master.zip
unzip master.zip
cd pcl-master
# Turn GPU and CUDA "ON" here
ccmake .
cmake .
cd gpu/containers
make
cd include/pcl/gpu/containers
cp impl/* .
cd ../..
cp -r gpu/ /usr/include/pcl-1.7/pcl/
cd ../../../../../
rm -rf master.zip pcl-master

# Create build directories
mkdir -p obj/cuda obj/HeadPoseEstimator obj/Mediators obj/Viewers

# Compile the code
make
mv /usr/include/pcl-1.7/pcl/gpu/ /tmp
make
mv /tmp/gpu /usr/include/pcl-1.7/pcl/
make

# Run
./KinectFusion
