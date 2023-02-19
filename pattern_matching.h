#ifndef PATTERN_MATCHING_H
#define PATTERN_MATCHING_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>

#include <iostream>

using namespace cv;
using namespace std;

#define VISION_TOLERANCE 0.0000001
#define D2R (CV_PI / 180.0)
#define R2D (180.0 / CV_PI)
#define MATCH_CANDIDATE_NUM 5

#define SUBITEM_INDEX 0
#define SUBITEM_SCORE 1
#define SUBITEM_ANGLE 2
#define SUBITEM_POS_X 3
#define SUBITEM_POS_Y 4

#define MAX_SCALE_TIMES 10
#define MIN_SCALE_TIMES 0
#define SCALE_RATIO 1.25

#define FONT_SIZE 115

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

struct s_TemplData
{
    vector<Mat> vecPyramid;
    vector<Scalar> vecTemplMean;
    vector<double> vecTemplNorm;
    vector<double> vecInvArea;
    vector<bool> vecResultEqual1;
    bool bIsPatternLearned;
    int iBorderColor;
    void clear ()
    {
        vector<Mat> ().swap (vecPyramid);
        vector<double> ().swap (vecTemplNorm);
        vector<double> ().swap (vecInvArea);
        vector<Scalar> ().swap (vecTemplMean);
        vector<bool> ().swap (vecResultEqual1);
    }
    void resize (int iSize)
    {
        vecTemplMean.resize (iSize);
        vecTemplNorm.resize (iSize, 0);
        vecInvArea.resize (iSize, 1);
        vecResultEqual1.resize (iSize, false);
    }
    s_TemplData ()
    {
        bIsPatternLearned = false;
    }
};

struct s_MatchParameter
{
    Point2d pt;
    double dMatchScore;
    double dMatchAngle;
    //Mat matRotatedSrc;
    Rect rectRoi;
    double dAngleStart;
    double dAngleEnd;
    RotatedRect rectR;
    Rect rectBounding;
    bool bDelete;

    double vecResult[3][3];//for subpixel
    int iMaxScoreIndex;//for subpixel
    bool bPosOnBorder;
    Point2d ptSubPixel;
    double dNewAngle;

    s_MatchParameter (Point2f ptMinMax, double dScore, double dAngle)//, Mat matRotatedSrc = Mat ())
    {
        pt = ptMinMax;
        dMatchScore = dScore;
        dMatchAngle = dAngle;

        bDelete = false;
        dNewAngle = 0.0;

        bPosOnBorder = false;
    }
    s_MatchParameter ()
    {
        double dMatchScore = 0;
        double dMatchAngle = 0;
    }
    ~s_MatchParameter ()
    {

    }
};
struct s_SingleTargetMatch
{
    Point2d ptLT, ptRT, ptRB, ptLB, ptCenter;
    double dMatchedAngle;
    double dMatchScore;
};
struct s_BlockMax
{
    struct Block
    {
        Rect rect;
        double dMax;
        Point ptMaxLoc;
        Block ()
        {}
        Block (Rect rect_, double dMax_, Point ptMaxLoc_)
        {
            rect = rect_;
            dMax = dMax_;
            ptMaxLoc = ptMaxLoc_;
        }
    };
    s_BlockMax ()
    {}
    vector<Block> vecBlock;
    Mat matSrc;
    s_BlockMax (Mat matSrc_, Size sizeTemplate)
    {
        matSrc = matSrc_;
        //將matSrc 拆成數個block，分別計算最大值
        int iBlockW = sizeTemplate.width * 2;
        int iBlockH = sizeTemplate.height * 2;

        int iCol = matSrc.cols / iBlockW;
        bool bHResidue = matSrc.cols % iBlockW != 0;

        int iRow = matSrc.rows / iBlockH;
        bool bVResidue = matSrc.rows % iBlockH != 0;

        if (iCol == 0 || iRow == 0)
        {
            vecBlock.clear ();
            return;
        }

        vecBlock.resize (iCol * iRow);
        int iCount = 0;
        for (int y = 0; y < iRow ; y++)
        {
            for (int x = 0; x < iCol; x++)
            {
                Rect rectBlock (x * iBlockW, y * iBlockH, iBlockW, iBlockH);
                vecBlock[iCount].rect = rectBlock;
                minMaxLoc (matSrc (rectBlock), 0, &vecBlock[iCount].dMax, 0, &vecBlock[iCount].ptMaxLoc);
                vecBlock[iCount].ptMaxLoc += rectBlock.tl ();
                iCount++;
            }
        }
        if (bHResidue && bVResidue)
        {
            Rect rectRight (iCol * iBlockW, 0, matSrc.cols - iCol * iBlockW, matSrc.rows);
            Block blockRight;
            blockRight.rect = rectRight;
            minMaxLoc (matSrc (rectRight), 0, &blockRight.dMax, 0, &blockRight.ptMaxLoc);
            blockRight.ptMaxLoc += rectRight.tl ();
            vecBlock.push_back (blockRight);

            Rect rectBottom (0, iRow * iBlockH, iCol * iBlockW, matSrc.rows - iRow * iBlockH);
            Block blockBottom;
            blockBottom.rect = rectBottom;
            minMaxLoc (matSrc (rectBottom), 0, &blockBottom.dMax, 0, &blockBottom.ptMaxLoc);
            blockBottom.ptMaxLoc += rectBottom.tl ();
            vecBlock.push_back (blockBottom);
        }
        else if (bHResidue)
        {
            Rect rectRight (iCol * iBlockW, 0, matSrc.cols - iCol * iBlockW, matSrc.rows);
            Block blockRight;
            blockRight.rect = rectRight;
            minMaxLoc (matSrc (rectRight), 0, &blockRight.dMax, 0, &blockRight.ptMaxLoc);
            blockRight.ptMaxLoc += rectRight.tl ();
            vecBlock.push_back (blockRight);
        }
        else
        {
            Rect rectBottom (0, iRow * iBlockH, matSrc.cols, matSrc.rows - iRow * iBlockH);
            Block blockBottom;
            blockBottom.rect = rectBottom;
            minMaxLoc (matSrc (rectBottom), 0, &blockBottom.dMax, 0, &blockBottom.ptMaxLoc);
            blockBottom.ptMaxLoc += rectBottom.tl ();
            vecBlock.push_back (blockBottom);
        }
    }
    void UpdateMax (Rect rectIgnore)
    {
        if (vecBlock.size () == 0)
            return;
        //找出所有跟rectIgnore交集的block
        int iSize = vecBlock.size ();
        for (int i = 0; i < iSize ; i++)
        {
            Rect rectIntersec = rectIgnore & vecBlock[i].rect;
            //無交集
            if (rectIntersec.width == 0 && rectIntersec.height == 0)
                continue;
            //有交集，更新極值和極值位置
            minMaxLoc (matSrc (vecBlock[i].rect), 0, &vecBlock[i].dMax, 0, &vecBlock[i].ptMaxLoc);
            vecBlock[i].ptMaxLoc += vecBlock[i].rect.tl ();
        }
    }
    void GetMaxValueLoc (double& dMax, Point& ptMaxLoc)
    {
        int iSize = vecBlock.size ();
        if (iSize == 0)
        {
            minMaxLoc (matSrc, 0, &dMax, 0, &ptMaxLoc);
            return;
        }
        //從block中找最大值
        int iIndex = 0;
        dMax = vecBlock[0].dMax;
        for (int i = 1 ; i < iSize; i++)
        {
            if (vecBlock[i].dMax >= dMax)
            {
                iIndex = i;
                dMax = vecBlock[i].dMax;
            }
        }
        ptMaxLoc = vecBlock[iIndex].ptMaxLoc;
    }
};


class pattern_matching
{
public:
    pattern_matching();

    void initial();
    void run();
    void LoadSrc();
    void LoadDst();
    void SaveRes();
    void CalculateAnomalyMap();
    void LearnPattern();
    bool Match();
    int GetTopLayer (Mat* matTempl, int iMinDstLength);
    void MatchTemplate (cv::Mat& matSrc, s_TemplData* pTemplData, cv::Mat& matResult, int iLayer, bool bUseSIMD);
    void GetRotatedROI (Mat& matSrc, Size size, Point2f ptLT, double dAngle, Mat& matROI);
    void CCOEFF_Denominator (cv::Mat& matSrc, s_TemplData* pTemplData, cv::Mat& matResult, int iLayer);
    Size GetBestRotationSize (Size sizeSrc, Size sizeDst, double dRAngle);
    Point2f ptRotatePt2f (Point2f ptInput, Point2f ptOrg, double dAngle);
    void FilterWithScore (vector<s_MatchParameter>* vec, double dScore);
    void FilterWithRotatedRect (vector<s_MatchParameter>* vec, int iMethod = CV_TM_CCOEFF_NORMED, double dMaxOverLap = 0);
    Point GetNextMaxLoc (Mat & matResult, Point ptMaxLoc, Size sizeTemplate, double& dMaxValue, double dMaxOverlap);
    Point GetNextMaxLoc (Mat & matResult, Point ptMaxLoc, Size sizeTemplate, double& dMaxValue, double dMaxOverlap, s_BlockMax& blockMax);
    void SortPtWithCenter (vector<Point2f>& vecSort);
    vector<s_SingleTargetMatch> m_vecSingleTargetData;
    void DrawDashLine (Mat& matDraw, Point ptStart, Point ptEnd, Scalar color1 = Scalar (0, 0, 255), Scalar color2 = Scalar::all (255));
    void DrawMarkCross (Mat& matDraw, int iX, int iY, int iLength, Scalar color, int iThickness);
    void OutputRoi (s_SingleTargetMatch ss);
    bool SubPixEsimation (vector<s_MatchParameter>* vec, double* dX, double* dY, double* dAngle, double dAngleStep, int iMaxScoreIndex);
    Mat diffFilterOfGrayImage(Mat back_img, Mat fore_img, int threshold, int k_size=3);
    Mat diffFilterOfColorImage(Mat back_img, Mat fore_img, int threshold, int k_size=3);
    Mat minFilterOfGrayImage(Mat img, int k_size=3);

    String src_path = "/Users/wangjb/Documents/Project/pattern_matching/qfn/qfn-src-1.bmp";
    String dst_path = "/Users/wangjb/Documents/Project/pattern_matching/qfn/qfn-dst-1-3.bmp";
    String out_path = "/Users/wangjb/Documents/Project/pattern_matching/qfn/qfn-out";
    Mat m_matSrc, matSrc_bgr, matSrc_cutcell;
    Mat m_matDst, matDst_bgr;
    s_TemplData m_TemplData;
    int m_iMaxPos = 200;
    double m_dMaxOverlap = 0.2;
    double m_dScore = 0.5;
    double m_dToleranceAngle = 5;
    int m_iMinReduceArea = 256;
    int k_size = 5;
    int pixel_threshold = 30;
    int m_dDstScale = 1;
    int m_dSrcScale = 1;
    int m_dNewScale = 1;
    bool m_bShowResult = false;
    bool m_bToleranceRange = false;
    int m_dTolerance1 = 40;
    int m_dTolerance2 = 60;
    int m_dTolerance3 = -110;
    int m_dTolerance4 = -100;
    bool m_bDebugMode = false;
    bool bSubPixelEstimation = true;
};


#endif // PATTERN_MATCHING_H
