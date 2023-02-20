#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#include <iostream>

using namespace cv;
using namespace std;

class imgprocess
{
public:
    imgprocess();

    Mat diffFilterOfColorImage(Mat back_img, Mat fore_img, int threshold, int k_size);
    Mat diffFilterOfGrayImage(Mat back_img, Mat fore_img, int threshold, int k_size);
    Mat minFilterOfGrayImage(Mat img, int k_size);
    Mat SimplestCB(Mat &in, float percent);
};

#endif // IMGPROCESS_H
