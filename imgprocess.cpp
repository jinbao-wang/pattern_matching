#include "imgprocess.h"

imgprocess::imgprocess()
{

}

Mat imgprocess::diffFilterOfColorImage(Mat back_img, Mat fore_img, int threshold, int k_size)
{
    Mat img_prev_gray;
    Mat img_main_gray;
    cvtColor(back_img, img_prev_gray, COLOR_RGB2GRAY);
    cvtColor(fore_img, img_main_gray, COLOR_RGB2GRAY);

    Mat img_diff = diffFilterOfGrayImage(img_prev_gray, img_main_gray, threshold, k_size);

    return img_diff;
}

Mat imgprocess::diffFilterOfGrayImage(Mat back_img, Mat fore_img, int threshold, int k_size)
{
    int max_rows = fore_img.rows;          // 行像素数
    int max_cols = fore_img.cols;          // 列像素数
    int p = (k_size - 1) / 2;              // -(k−1)/2 ~ (k−1)/2
    Mat img_diff = Mat::zeros(back_img.rows, back_img.cols, CV_8UC1);

    // 对每个像素点进行处理
    for (int row = 0; row < max_rows; ++row) {
        for (int col = 0; col < max_cols; ++col) {
            int fore_point = fore_img.at<uchar>(row, col);
            int diff_min = 255; // 初始最大值
            // 以当前像素点为中心的k*k的矩阵中，取最小值
            for (int i = row - p; i <= row + p; ++i)
                for (int j = col - p; j <= col + p; ++j)
                    if(i >= 0 && i < max_rows && j >= 0 && j < max_cols){
                        int back_point = back_img.at<uchar>(i, j);
                        int diff = cv::abs(back_point - fore_point);
                        if(diff < diff_min){
                            diff_min = diff;
                        }
                        if(diff_min > threshold){
                            diff_min = 255;
                        }else{
                            diff_min = 0;
                        }
                        img_diff.at<uchar>(row, col) = diff_min;
                    }
        }
    }

    return img_diff;
}

Mat imgprocess::minFilterOfGrayImage(Mat img, int k_size)
{
    int max_rows = img.rows;          // 行像素数
    int max_cols = img.cols;          // 列像素数
    int p = (k_size - 1) / 2;               // -(k−1)/2 ~ (k−1)/2
    cv::Mat dst = cv::Mat::zeros(max_rows, max_cols, CV_8UC1);

    // 对每个像素点进行处理
    for (int row = 0; row < max_rows; ++row) {
        for (int col = 0; col < max_cols; ++col) {
            int mint = 255; // 初始最大值
            // 以当前像素点为中心的k*k的矩阵中，取最小值
            for (int i = row - p; i <= row + p; ++i)
                for (int j = col - p; j <= col + p; ++j)
                    if(i >= 0 && i < max_rows && j >= 0 && j < max_cols)
                        if (img.at<uchar>(i, j) < mint)
                            mint = img.at<uchar>(i, j);
            dst.at<uchar>(row, col) = mint;   // 像素值赋最小值
        }
    }

    return dst;
}

/// perform the Simplest Color Balancing algorithm
Mat imgprocess::SimplestCB(Mat &in, float percent)
{
    assert(in.channels() == 3);
    assert(percent > 0 && percent < 100);
    Mat out;
    float half_percent = percent / 200.0f;

    vector<Mat> tmpsplit; split(in,tmpsplit);
    for(int i=0;i<3;i++) {
        //find the low and high precentile values (based on the input percentile)
        Mat flat; tmpsplit[i].reshape(1,1).copyTo(flat);
        cv::sort(flat,flat,CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
        int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
        int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
        //cout << lowval << " " << highval << endl;

        //saturate below the low percentile and above the high percentile
        tmpsplit[i].setTo(lowval,tmpsplit[i] < lowval);
        tmpsplit[i].setTo(highval,tmpsplit[i] > highval);

        //scale the channel
        normalize(tmpsplit[i],tmpsplit[i],0,255,NORM_MINMAX);
    }
    merge(tmpsplit,out);

    return out;
}
