#ifndef VISUALIZE_H
#define VISUALIZE_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#include <iostream>

using namespace cv;
using namespace std;

struct color
{



};
const Scalar colorWaterBlue (230, 255, 102);
const Scalar colorBlue (255, 0, 0);
const Scalar colorYellow (0, 255, 255);
const Scalar colorRed (0, 0, 255);
const Scalar colorBlack (0, 0, 0);
const Scalar colorGray (200, 200, 200);
const Scalar colorSystem (240, 240, 240);
const Scalar colorGreen (0, 255, 0);
const Scalar colorWhite (255, 255, 255);
const Scalar colorPurple (214, 112, 218);
const Scalar colorGoldenrod (15, 185, 255);

struct s_SingleTargetMatch
{
    Point2d ptLT, ptRT, ptRB, ptLB, ptCenter;
    double dMatchedAngle;
    double dMatchScore;
};

class visualize
{
public:
    visualize();

    void drawBox(Mat& matSrc_bgr, Mat& m_matTpl, s_SingleTargetMatch& target, int i);
    void DrawDashLine(Mat &matDraw, Point ptStart, Point ptEnd, Scalar color1, Scalar color2);
    void DrawMarkCross(Mat &matDraw, int iX, int iY, int iLength, Scalar color, int iThickness);


};

#endif // VISUALIZE_H
