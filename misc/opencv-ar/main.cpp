#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>

void merge(cv::Mat &in1, cv::Mat &in2,cv::Mat &out)
{
    int nLines = in1.rows;
    int nc = in1.cols * in1.channels();
    if(in1.isContinuous())
    {
        nc = nc*nLines;
        nLines = 1;
    }
    for(int j=0;j<nLines;j++)
    {
        uchar* dataIN1 = in1.ptr<uchar>(j); //fajne :)
        uchar* dataIN2 = in2.ptr<uchar>(j);
        uchar* dataOUT = out.ptr<uchar>(j);
        for(int i=0;i<nc; i++)
        {
            if(dataIN2[i] == 0)
            {
                dataOUT[i] = dataIN1[i];
            }
            else
            {
                dataOUT[i] = dataIN2[i];
            }

        }
    }
}

int main(int argc, char *argv[])
{
    const int boardWidth = 5;
    const int boardHeight = 4;
    const cv::Size boardSize = cv::Size(boardWidth,boardHeight);
    std::vector<cv::Point> corners;
    cv::Point2f p[4],q[4];
    cv::Mat frame;
    cv::Mat pic = cv::imread("kot.bmp");
    cv::flip(pic,pic,0);
    cv::flip(pic,pic,1);
    cv::Mat overlay = cv::Mat::zeros(800,600,CV_8UC3);
    cv::Mat perspMat;
    cv::VideoCapture cap(1);
    if (!cap.isOpened())  // check if we succeeded
      return -1;
    cv::VideoWriter write;
    write.open("augment.avi",CV_FOURCC('M','J','P','G'),15,cv::Size(800,600),true);
    cv::namedWindow("Frame",0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);
    for(;;)
    {
        cap >> frame;
        if(!frame.data)
            break;
        bool found = cv::findChessboardCorners(frame,boardSize,corners,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        if(found)
        {
            q[0].x= (float) 0;
            q[0].y= (float) 0;
            q[1].x= (float) overlay.cols;
            q[1].y= (float) 0;

            q[2].x= (float) overlay.cols;
            q[2].y= (float) overlay.rows;
            q[3].x= (float) 0;
            q[3].y= (float) overlay.rows;

            p[0].x= corners[0].x;
            p[0].y= corners[0].y;
            p[1].x= corners[4].x;
            p[1].y= corners[4].y;

            p[2].x= corners[19].x;
            p[2].y= corners[19].y;
            p[3].x= corners[15].x;
            p[3].y= corners[15].y;
            perspMat = cv::getPerspectiveTransform(q,p);
            cv::warpPerspective(pic,overlay,perspMat,cv::Size(frame.cols,frame.rows));
            merge(frame,overlay,frame);

        }
        cv::imshow("Frame",frame);
        write.write(frame);
        if(cv::waitKey(30) >= 0) break;
    }
    return 0;
}
