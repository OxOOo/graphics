#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// BGR
typedef unsigned char uint8;

void SeamCarving(Mat& dst, Mat src, Mat kernel, const int output_height, const int output_width)
{
    const int MaxW = max(src.cols, output_width);
    const int MaxH = max(src.rows, output_height);

    Mat E(MaxH, MaxW, CV_64F);
    Mat DP(MaxH, MaxW, CV_64F);
    Mat PATH(MaxH, MaxW, CV_32S);
    Mat MASK(MaxH, MaxW, CV_32S);

    while(src.cols > output_width)
    {
        Mat dx, dy;
        filter2D(src, dx, -1, kernel);
        filter2D(src, dy, -1, kernel.t());

        // calc E
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0; j < src.cols; j ++)
            {
                int sx = 0, sy = 0;
                for(int k = 0; k < 3; k ++)
                {
                    sx += dx.at<Vec3b>(i, j)[k]*dx.at<Vec3b>(i, j)[k];
                    sy += dy.at<Vec3b>(i, j)[k]*dy.at<Vec3b>(i, j)[k];
                }
                E.at<double>(i, j) = sqrt(sx) + sqrt(sy);
            }
        
        // calc DP & PATH
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0; j < src.cols; j ++)
            {
                DP.at<double>(i, j) = E.at<double>(i, j);
                PATH.at<int>(i, j) = 0;
                if (i > 0)
                {
                    DP.at<double>(i, j) = E.at<double>(i, j) + DP.at<double>(i-1, j);
                    PATH.at<int>(i, j) = j;
                    if (j > 0 && DP.at<double>(i, j) > E.at<double>(i, j) + DP.at<double>(i-1, j-1))
                    {
                        DP.at<double>(i, j) = E.at<double>(i, j) + DP.at<double>(i-1, j-1);
                        PATH.at<int>(i, j) = j-1;
                    }
                    if (j < src.cols-1 && DP.at<double>(i, j) > E.at<double>(i, j) + DP.at<double>(i-1, j+1))
                    {
                        DP.at<double>(i, j) = E.at<double>(i, j) + DP.at<double>(i-1, j+1);
                        PATH.at<int>(i, j) = j+1;
                    }
                }
            }
        
        double min_e = 1e10;
        int min_j;
        for(int j = 0; j < src.cols; j ++)
            if (min_e > DP.at<double>(src.rows-1, j))
            {
                min_e = DP.at<double>(src.rows-1, j);
                min_j = j;
            }
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0; j < src.cols; j ++)
                MASK.at<int>(i, j) = 0;
        for(int i = src.rows-1; i >= 0; i --)
        {
            MASK.at<int>(i, min_j) = 1;
            min_j = PATH.at<int>(i, min_j);
        }
        // =======debug========
        Mat img(src.rows, src.cols, CV_8UC3);
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0; j < src.cols; j ++)
            {
                img.at<Vec3b>(i, j) = src.at<Vec3b>(i, j);
                if (MASK.at<int>(i, j) == 1)
                {
                    img.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
                }
            }
        char buf[100];
        sprintf(buf, "../seam_images/debug%d.png", src.cols);
        imwrite(buf, img);

        dst = Mat::zeros(src.rows, src.cols-1, CV_8UC3);
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0, offset = 0; j < src.cols; j ++)
                if (MASK.at<int>(i, j) == 0)
                {
                    dst.at<Vec3b>(i, offset) = src.at<Vec3b>(i, j);
                    if (j > 0 && MASK.at<int>(i, j-1) == 1)
                    {
                        for(int k = 0; k < 3; k ++)
                            dst.at<Vec3b>(i, offset)[k] = (int(src.at<Vec3b>(i, j)[k]) + int(src.at<Vec3b>(i, j-1)[k]))/2;
                    }
                    if (j+1 < src.cols && MASK.at<int>(i, j+1) == 1)
                    {
                        for(int k = 0; k < 3; k ++)
                            dst.at<Vec3b>(i, offset)[k] = (int(src.at<Vec3b>(i, j)[k]) + int(src.at<Vec3b>(i, j+1)[k]))/2;
                    }
                    offset ++;
                }
        dst.copyTo(src);
    }

    src.copyTo(dst);
}

int main()
{
    Mat img = imread("../seam_images/1.jpg", CV_LOAD_IMAGE_COLOR);
    Mat dst;
    SeamCarving(dst, img, (Mat_<double>(1, 3) << -1, 1, 0), img.rows, img.cols-200);
    imwrite("../seam_images/temp.png", dst);

    cout << "============" << endl;

    return 0;
}