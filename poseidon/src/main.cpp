#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <functional>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// BGR
typedef unsigned char uint8;

Mat getColMask(const Mat& src, const Mat& delta, std::function<void(const Mat &src, Mat &E)> E_func)
{
    Mat E(src.rows, src.cols, CV_32S);
    Mat DP(src.rows, src.cols, CV_32S);
    Mat PATH(src.rows, src.cols, CV_32S);
    Mat MASK = Mat::zeros(src.rows, src.cols, CV_32S);

    E_func(src, E);
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
            E.at<int>(i, j) += delta.at<int>(i, j);
    
    // calc DP & PATH
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
        {
            DP.at<int>(i, j) = E.at<int>(i, j);
            PATH.at<int>(i, j) = 0;
            if (i > 0)
            {
                DP.at<int>(i, j) = E.at<int>(i, j) + DP.at<int>(i-1, j);
                PATH.at<int>(i, j) = j;
                if (j > 0 && DP.at<int>(i, j) > E.at<int>(i, j) + DP.at<int>(i-1, j-1))
                {
                    DP.at<int>(i, j) = E.at<int>(i, j) + DP.at<int>(i-1, j-1);
                    PATH.at<int>(i, j) = j-1;
                }
                if (j < src.cols-1 && DP.at<int>(i, j) > E.at<int>(i, j) + DP.at<int>(i-1, j+1))
                {
                    DP.at<int>(i, j) = E.at<int>(i, j) + DP.at<int>(i-1, j+1);
                    PATH.at<int>(i, j) = j+1;
                }
            }
        }
    
    int min_e = 1e9;
    int min_j;
    for(int j = 0; j < src.cols; j ++)
        if (min_e > DP.at<int>(src.rows-1, j))
        {
            min_e = DP.at<int>(src.rows-1, j);
            min_j = j;
        }
    for(int i = src.rows-1; i >= 0; i --)
    {
        MASK.at<int>(i, min_j) = 1;
        min_j = PATH.at<int>(i, min_j);
    }
    return MASK;
}

Mat deleteCol(const Mat& src, const Mat& MASK)
{
    Mat dst = Mat::zeros(src.rows, src.cols-1, CV_8UC3);
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0, offset = 0; j < src.cols; j ++)
            if (MASK.at<int>(i, j) == 0)
            {
                dst.at<Vec3b>(i, offset) = src.at<Vec3b>(i, j);
                offset ++;
            }
    return dst;
}

Mat insertCol(const Mat& src, const Mat& MASK)
{
    Mat dst = Mat::zeros(src.rows, src.cols+1, CV_8UC3);
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0, offset = 0; j < src.cols; j ++)
            if (MASK.at<int>(i, j) == 0)
            {
                dst.at<Vec3b>(i, offset) = src.at<Vec3b>(i, j);
                offset ++;
            } else {
                if (j == 0 || j == src.cols-1) {
                    dst.at<Vec3b>(i, offset) = dst.at<Vec3b>(i, offset+1) = src.at<Vec3b>(i, j);
                } else {
                    Vec3b s = src.at<Vec3b>(i, j-1), m = src.at<Vec3b>(i, j), t = src.at<Vec3b>(i, j+1);
                    for(int k = 0; k < 3; k ++)
                    {
                        dst.at<Vec3b>(i, offset)[k] = (int(s[k]) + int(m[k]))/2;
                        dst.at<Vec3b>(i, offset+1)[k] = (int(m[k]) + int(t[k]))/2;
                    }
                }
                offset += 2;
            }
    return dst;
}

Mat getColDelta(const Mat& delta, const Mat& MASK)
{
    Mat dst_delta = Mat::zeros(delta.rows, delta.cols+1, CV_32S);
    for(int i = 0; i < delta.rows; i ++)
        for(int j = 0, offset = 0; j < delta.cols; j ++)
            if (MASK.at<int>(i, j) == 0)
            {
                dst_delta.at<int>(i, offset) = delta.at<int>(i, j)*0.9;
                offset ++;
            } else {
                dst_delta.at<int>(i, offset) = dst_delta.at<int>(i, offset+1) = 1e5;
                offset += 2;
            }
    return dst_delta;
}

Mat getRowMask(const Mat& src, const Mat& delta, std::function<void(const Mat &src, Mat &E)> E_func)
{
    Mat E(src.rows, src.cols, CV_32S);
    Mat DP(src.rows, src.cols, CV_32S);
    Mat PATH(src.rows, src.cols, CV_32S);
    Mat MASK = Mat::zeros(src.rows, src.cols, CV_32S);

    E_func(src, E);
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
            E.at<int>(i, j) += delta.at<int>(i, j);
    
    // calc DP & PATH
    for(int j = 0; j < src.cols; j ++)
        for(int i = 0; i < src.rows; i ++)
        {
            DP.at<int>(i, j) = E.at<int>(i, j);
            PATH.at<int>(i, j) = 0;
            if (j > 0)
            {
                DP.at<int>(i, j) = E.at<int>(i, j) + DP.at<int>(i, j-1);
                PATH.at<int>(i, j) = i;
                if (i > 0 && DP.at<int>(i, j) > E.at<int>(i, j) + DP.at<int>(i-1, j-1))
                {
                    DP.at<int>(i, j) = E.at<int>(i, j) + DP.at<int>(i-1, j-1);
                    PATH.at<int>(i, j) = i-1;
                }
                if (i < src.rows-1 && DP.at<int>(i, j) > E.at<int>(i, j) + DP.at<int>(i+1, j-1))
                {
                    DP.at<int>(i, j) = E.at<int>(i, j) + DP.at<int>(i+1, j-1);
                    PATH.at<int>(i, j) = i+1;
                }
            }
        }
    
    int min_e = 1e9;
    int min_i;
    for(int i = 0; i < src.rows; i ++)
        if (min_e > DP.at<int>(i, src.cols-1))
        {
            min_e = DP.at<int>(i, src.cols-1);
            min_i = i;
        }
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
            MASK.at<int>(i, j) = 0;
    for(int j = src.cols-1; j >= 0; j --)
    {
        MASK.at<int>(min_i, j) = 1;
        min_i = PATH.at<int>(min_i, j);
    }

    return MASK;
}

Mat deleteRow(const Mat& src, const Mat& MASK)
{
    Mat dst = Mat::zeros(src.rows-1, src.cols, CV_8UC3);
    for(int j = 0; j < src.cols; j ++)
        for(int i = 0, offset = 0; i < src.rows; i ++)
            if (MASK.at<int>(i, j) == 0)
            {
                dst.at<Vec3b>(offset, j) = src.at<Vec3b>(i, j);
                offset ++;
            }
    return dst;
}

Mat insertRow(const Mat& src, const Mat& MASK)
{
    Mat dst = Mat::zeros(src.rows+1, src.cols, CV_8UC3);
    for(int j = 0; j < src.cols; j ++)
        for(int i = 0, offset = 0; i < src.rows; i ++)
            if (MASK.at<int>(i, j) == 0)
            {
                dst.at<Vec3b>(offset, j) = src.at<Vec3b>(i, j);
                offset ++;
            } else {
                if (i == 0 || i == src.rows-1) {
                    dst.at<Vec3b>(offset, j) = dst.at<Vec3b>(offset+1, j) = src.at<Vec3b>(i, j);
                } else {
                    Vec3b s = src.at<Vec3b>(i-1, j), m = src.at<Vec3b>(i, j), t = src.at<Vec3b>(i+1, j);
                    for(int k = 0; k < 3; k ++)
                    {
                        dst.at<Vec3b>(offset, j)[k] = (int(s[k]) + int(m[k]))/2;
                        dst.at<Vec3b>(offset+1, j)[k] = (int(m[k]) + int(t[k]))/2;
                    }
                }
                offset += 2;
            }
    return dst;
}

Mat getRowDelta(const Mat& delta, const Mat& MASK)
{
    Mat dst_delta = Mat::zeros(delta.rows+1, delta.cols, CV_32S);
    for(int j = 0; j < delta.cols; j ++)
        for(int i = 0, offset = 0; i < delta.rows; i ++)
            if (MASK.at<int>(i, j) == 0)
            {
                dst_delta.at<int>(offset, j) = delta.at<int>(i, j)*0.9;
                offset ++;
            } else {
                dst_delta.at<int>(offset, j) = dst_delta.at<int>(offset+1, j) = 1e5;
                offset += 2;
            }
    return dst_delta;
}

void SeamCarving(Mat src, Mat& dst, std::function<void(const Mat &src, Mat &E)> E_func, const int output_height, const int output_width)
{
    Mat delta = Mat::zeros(src.rows, src.cols, CV_32S);

    while(src.cols != output_width || src.rows != output_height)
    {
        if (src.cols != output_width)
        {
            Mat MASK = getColMask(src, delta, E_func);

            // ===========debug===============
            // Mat img(src.rows, src.cols, CV_8UC3);
            // for(int i = 0; i < src.rows; i ++)
            //     for(int j = 0; j < src.cols; j ++)
            //     {
            //         img.at<Vec3b>(i, j) = src.at<Vec3b>(i, j);
            //         if (MASK.at<int>(i, j) == 1)
            //         {
            //             img.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
            //         }
            //     }
            // char buf[100];
            // sprintf(buf, "../seam_images/debug-cols%d.png", src.cols);
            // imwrite(buf, img);

            if (src.cols > output_width)
            {
                src = deleteCol(src, MASK);
            } else {
                src = insertCol(src, MASK);
                delta = getColDelta(delta, MASK);
            }
        }

        if (src.rows != output_height)
        {
            Mat MASK = getRowMask(src, delta, E_func);

            if (src.rows > output_height)
            {
                src = deleteRow(src, MASK);
            } else {
                src = insertRow(src, MASK);
                delta = getRowDelta(delta, MASK);
            }
        }
    }

    src.copyTo(dst);
}

class EOperator
{
private:
    Mat kernel;
public:
    EOperator(Mat kernel): kernel(kernel) {}
    void operator()(const Mat &src, Mat &E)
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
                E.at<int>(i, j) = sqrt(sx) + sqrt(sy);
            }
    }
};

int main()
{
    const Mat basic_kernel = (Mat_<int>(1, 3) << -1, 1, 0);
    const Mat sobel_kernel = (Mat_<int>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
    const Mat laplacian_kernel = (Mat_<int>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
    
    std::function<void(const Mat&, Mat&)> basic_func = EOperator(basic_kernel);
    std::function<void(const Mat&, Mat&)> laplacian_func = [](const Mat &src, Mat &E) {
        Mat gray_src;
        cvtColor(src, gray_src, CV_BGR2GRAY);
        Mat dst;
        Laplacian(gray_src, dst, -1);
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0; j < src.cols; j ++)
            {
                E.at<int>(i, j) = dst.at<uint8>(i, j);
            }
    };
    std::function<void(const Mat&, Mat&)> gradient_func = [](const Mat &src, Mat &E) {
        Mat gray_src;
        cvtColor(src, gray_src, CV_BGR2GRAY);
        for(int i = 0; i < gray_src.rows; i ++)
            for(int j = 0; j < gray_src.cols; j ++)
                E.at<int>(i, j) = 0;
        for(int i = 0; i < gray_src.rows; i ++)
        {
            E.at<int>(i, 0) += abs(int(gray_src.at<uint8>(i, 1)) - int(gray_src.at<uint8>(i, 0))) * 2;
            for(int j = 1; j < gray_src.cols-1; j ++)
                E.at<int>(i, j) += abs(int(gray_src.at<uint8>(i, j+1)) - int(gray_src.at<uint8>(i, j-1)));
            E.at<int>(i, gray_src.cols-1) += abs(int(gray_src.at<uint8>(i, gray_src.cols-1)) - int(gray_src.at<uint8>(i, gray_src.cols-2))) * 2;
        }
        for(int j = 0; j < gray_src.cols; j ++)
        {
            E.at<int>(0, j) = abs(int(gray_src.at<uint8>(1, j)) - int(gray_src.at<uint8>(0, j))) * 2;
            for(int i = 1; i < gray_src.rows-1; i ++)
                E.at<int>(i, j) = abs(int(gray_src.at<uint8>(i+1, j)) - int(gray_src.at<uint8>(i-1, j)));
            E.at<int>(gray_src.rows-1, j) = abs(int(gray_src.at<uint8>(gray_src.rows-1, j)) - int(gray_src.at<uint8>(gray_src.rows-2, j))) * 2;
        }
        for(int i = 0; i < gray_src.rows; i ++)
            for(int j = 0; j < gray_src.cols; j ++)
                E.at<int>(i, j) = min(E.at<int>(i, j), 255);
    };

    Mat img = imread("../seam_images/4.jpg", CV_LOAD_IMAGE_COLOR);
    Mat dst;
    SeamCarving(img, dst, laplacian_func, img.rows+100, img.cols+100);
    imwrite("../seam_images/temp.png", dst);

    return 0;
}