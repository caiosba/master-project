/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2010, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: common.h 4426 2012-02-13 06:14:56Z rusu $
 *
 */
#ifndef PCL_PCL_VISUALIZER_COMMON_H_
#define PCL_PCL_VISUALIZER_COMMON_H_
#include <vtkCommand.h>
#include <vtkTextActor.h>

#include <pcl/pcl_macros.h>
#include <Eigen/Dense>

namespace pcl
{
  namespace visualization
  {
    /** \brief Get (good) random values for R/G/B.
      * \param r the resultant R color value
      * \param g the resultant G color value
      * \param b the resultant B color value
      * \param min minimum value for the colors
      * \param max maximum value for the colors
      */
    PCL_EXPORTS void
    getRandomColors (double &r, double &g, double &b, double min = 0.2, double max = 2.8);

    enum RenderingProperties
    {
      PCL_VISUALIZER_POINT_SIZE,
      PCL_VISUALIZER_OPACITY,
      PCL_VISUALIZER_LINE_WIDTH,
      PCL_VISUALIZER_FONT_SIZE,
      PCL_VISUALIZER_COLOR,
      PCL_VISUALIZER_REPRESENTATION,
      PCL_VISUALIZER_IMMEDIATE_RENDERING
    };

    enum RenderingRepresentationProperties
    {
      PCL_VISUALIZER_REPRESENTATION_POINTS,
      PCL_VISUALIZER_REPRESENTATION_WIREFRAME,
      PCL_VISUALIZER_REPRESENTATION_SURFACE
    };

    //////////////////////////////////////////////////////////////////////////////////////////////
    /** \brief Camera class holds a set of camera parameters together with the window pos/size. */
    class PCL_EXPORTS Camera
    {
      public:
        // focal point or lookAt
        double focal[3];

        // position of the camera
        double pos[3];

        // up vector of the camera
        double view[3];

        // clipping planes clip[0] is near clipping plane or also the image plane, and clip [1] is the far clipping plane
        double clip[2];

        // field of view angle in y direction (radians)
        double fovy;

        // the following variables are the actual position and size of the window on the screen
        // and NOT the viewport! except for the size, which is the same
        // the viewport is assumed to be centered and same size as the window.
        double window_size[2];
        double window_pos[2];

        /** \brief Computes View matrix for Camera (Based on gluLookAt)
          * \param[out] view_mat the resultant matrix
          */
        void 
        computeViewMatrix (Eigen::Matrix4d& view_mat) const;

        /** \brief Computes Projection Matrix for Camera
          *  \param[out] proj the resultant matrix
          */
        void 
        computeProjectionMatrix (Eigen::Matrix4d& proj) const;

        /** \brief converts point to window coordiantes
          * \param[in] pt xyz point to be converted
          * \param[out] window_cord vector containing the pts' window X,Y, Z and 1
          *
          * This function computes the projection and view matrix every time.
          * It is very inefficient to use this for every point in the point cloud!
          */
        template<typename PointT> void 
        cvtWindowCoordinates (const PointT& pt, Eigen::Vector4d& window_cord);

        /** \brief converts point to window coordiantes
          * \param[in] pt xyz point to be converted
          * \param[out] window_cord vector containing the pts' window X,Y, Z and 1
          * \param[in] composite_mat composite transformation matrix (proj*view)
          *
          * Use this function to compute window coordinates with a precomputed
          * transformation function.  The typical composite matrix will be
          * the projection matrix * the view matrix.  However, additional
          * matrices like a camera disortion matrix can also be added.
          */
        template<typename PointT> void 
        cvtWindowCoordinates (const PointT& pt, Eigen::Vector4d& window_cord, const Eigen::Matrix4d& composite_mat);
    };

    //////////////////////////////////////////////////////////////////////////////////////////////
    class PCL_EXPORTS FPSCallback : public vtkCommand
    {
      public:
        static FPSCallback *New () { return new FPSCallback;}
        inline void setTextActor (vtkTextActor *txt) { this->actor_ = txt; }
        virtual void Execute (vtkObject *, unsigned long, void*);
      protected:
        vtkTextActor *actor_;
    };
  }
}

#endif
