/*
 *  pandaViewer.h
 *  Created on: Mar 25, 2013
 *  Author: heekinho
 */

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

#include "texture.h"

void* panda_thread(void*);
void set_rgb_data(int width, int height, void* image, int f);
void set_depth_data(int width, int height, void* image, int f);

void set_mat(Mat m, int f);
void create_frame(int f);
