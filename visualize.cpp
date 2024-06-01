#include "visualize.h"

visualize::visualize()
{

}

void visualize::drawBox(Mat &matSrc_bgr, Mat &m_matTpl, s_SingleTargetMatch& target, int i)
{
    Point ptLT (target.ptLT);
    Point ptLB (target.ptLB);
    Point ptRB (target.ptRB);
    Point ptRT (target.ptRT);
    Point ptC (target.ptCenter);
    DrawDashLine (matSrc_bgr, ptLT, ptLB, colorRed, colorWhite);
    DrawDashLine (matSrc_bgr, ptLB, ptRB, colorRed, colorWhite);
    DrawDashLine (matSrc_bgr, ptRB, ptRT, colorRed, colorWhite);
    DrawDashLine (matSrc_bgr, ptRT, ptLT, colorRed, colorWhite);

    //左上及角落邊框
    Point ptDis1, ptDis2;
    if (m_matTpl.cols > m_matTpl.rows)
    {
        ptDis1 = (ptLB - ptLT) / 3;
        ptDis2 = (ptRT - ptLT) / 3 * (m_matTpl.rows / (float)m_matTpl.cols);
    }else {
        ptDis1 = (ptLB - ptLT) / 3 * (m_matTpl.cols / (float)m_matTpl.rows);
        ptDis2 = (ptRT - ptLT) / 3;
    }
    line (matSrc_bgr, ptLT, ptLT + ptDis1 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptLT, ptLT + ptDis2 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptRT, ptRT + ptDis1 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptRT, ptRT - ptDis2 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptRB, ptRB - ptDis1 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptRB, ptRB - ptDis2 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptLB, ptLB - ptDis1 / 2, colorGreen, 1, CV_AA);
    line (matSrc_bgr, ptLB, ptLB + ptDis2 / 2, colorGreen, 1, CV_AA);

    DrawDashLine (matSrc_bgr, ptLT + ptDis1, ptLT + ptDis2, colorRed, colorWhite);
    DrawMarkCross (matSrc_bgr, ptC.x, ptC.y, 5, colorGreen, 1);
    string str = format ("%d", i);
    putText (matSrc_bgr, str, (ptLT + ptRT) / 2, FONT_HERSHEY_PLAIN, 1, colorGreen);
}

void visualize::DrawDashLine(Mat &matDraw, Point ptStart, Point ptEnd, Scalar color1, Scalar color2)
{
    LineIterator itLine (matDraw, ptStart, ptEnd, 8, 0);
    int iCount = itLine.count;
    bool bOdd = false;
    for (int i = 0; i < iCount; i+=1, itLine++)
    {
        if (i % 3 == 0)
        {
            //白色BGR
            (*itLine)[0] = (uchar)color2.val[0];
            (*itLine)[1] = (uchar)color2.val[1];
            (*itLine)[2] = (uchar)color2.val[2];
        }
        else
        {
            //紅色BGR
            (*itLine)[0] = (uchar)color1.val[0];
            (*itLine)[1] = (uchar)color1.val[1];
            (*itLine)[2] = (uchar)color1.val[2];
        }

    }
}

void visualize::DrawMarkCross(Mat &matDraw, int iX, int iY, int iLength, Scalar color, int iThickness)
{
    if (matDraw.empty ())
        return;
    Point ptC (iX, iY);
    line (matDraw, ptC - Point (iLength, 0), ptC + Point (iLength, 0), color, iThickness);
    line (matDraw, ptC - Point (0, iLength), ptC + Point (0, iLength), color, iThickness);
}
