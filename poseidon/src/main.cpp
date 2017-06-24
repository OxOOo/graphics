#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <functional>
#include <opencv2/opencv.hpp>
#include <vector>

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

void maskSeams(Mat& seams, const Mat& originX, const Mat& originY, const Mat& MASK)
{
    for(int i = 0; i < MASK.rows; i ++)
        for(int j = 0; j < MASK.cols; j ++)
            if(MASK.at<int>(i, j) == 1)
            {
                seams.at<Vec3b>(originX.at<int>(i, j), originY.at<int>(i, j)) = Vec3b(0, 0, 255);
            }
}

Mat SeamCarving(Mat src, Mat& dst, std::function<void(const Mat &src, Mat &E)> E_func, const int output_height, const int output_width)
{
    Mat delta = Mat::zeros(src.rows, src.cols, CV_32S);
    Mat seams; src.copyTo(seams);
    Mat originX = Mat::zeros(src.rows, src.cols, CV_32S);
    Mat originY = Mat::zeros(src.rows, src.cols, CV_32S);
    bool needseams = src.cols >= output_width && src.rows >= output_height;

    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
        {
            originX.at<int>(i, j) = i;
            originY.at<int>(i, j) = j;
        }

    while(src.cols != output_width || src.rows != output_height)
    {
        if (src.cols != output_width)
        {
            Mat MASK = getColMask(src, delta, E_func);

            if (src.cols > output_width)
            {
                src = deleteCol(src, MASK);
            } else {
                src = insertCol(src, MASK);
                delta = getColDelta(delta, MASK);
            }

            if (needseams)
            {
                maskSeams(seams, originX, originY, MASK);
                for(int i = 0; i < src.rows; i ++)
                    for(int j = 0, offset = 0; j < src.cols; j ++)
                        if (MASK.at<int>(i, j) == 0)
                        {
                            originX.at<int>(i, offset) = originX.at<int>(i, j);
                            originY.at<int>(i, offset) = originY.at<int>(i, j);
                            offset ++;
                        }
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

            if (needseams)
            {
                maskSeams(seams, originX, originY, MASK);
                for(int j = 0; j < src.cols; j ++)
                    for(int i = 0, offset = 0; i < src.rows; i ++)
                        if (MASK.at<int>(i, j) == 0)
                        {
                            originX.at<int>(offset, j) = originX.at<int>(i, j);
                            originY.at<int>(offset, j) = originY.at<int>(i, j);
                            offset ++;
                        }
            }
        }
    }

    src.copyTo(dst);
    return seams;
}

Mat RemoveItem(Mat src, Mat& dst, std::function<void(const Mat &src, Mat &E)> E_func, Vec3b flag)
{
    Mat seams; src.copyTo(seams);
    Mat originX = Mat::zeros(src.rows, src.cols, CV_32S);
    Mat originY = Mat::zeros(src.rows, src.cols, CV_32S);

    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
        {
            originX.at<int>(i, j) = i;
            originY.at<int>(i, j) = j;
        }

    Mat removing = Mat::zeros(src.rows, src.cols, CV_32S);
    for(int i = 0; i < src.rows; i ++)
        for(int j = 0; j < src.cols; j ++)
        {
            const int dx[] = {0, 0, 1, -1, 0};
            const int dy[] = {1, -1, 0, 0, 0};
            int nums = 0;
            for(int d = 0; d < 5; d ++)
            {
                int x = i + dx[d], y = j + dy[d];
                if (0 <= x && x < src.rows && 0 <= y && y < src.cols && src.at<Vec3b>(x, y) == flag)
                    nums ++;
            }
            if (nums > 0) removing.at<int>(i, j) = 1;
        }

    while(true)
    {
        Mat delta = Mat::zeros(src.rows, src.cols, CV_32S);
        int nums = 0;
        for(int i = 0; i < src.rows; i ++)
            for(int j = 0; j < src.cols; j ++)
                if (removing.at<int>(i, j))
                {
                    nums ++;
                    delta.at<int>(i, j) = -1e5;
                }
        if (nums > 0)
        {
            Mat MASK = getColMask(src, delta, E_func);

            src = deleteCol(src, MASK);
            for(int i = 0; i < src.rows; i ++)
                for(int j = 0, offset = 0; j < src.cols; j ++)
                    if (MASK.at<int>(i, j) == 0)
                    {
                        removing.at<int>(i, offset) = removing.at<int>(i, j);
                        offset ++;
                    }
            
            maskSeams(seams, originX, originY, MASK);
            for(int i = 0; i < src.rows; i ++)
                for(int j = 0, offset = 0; j < src.cols; j ++)
                    if (MASK.at<int>(i, j) == 0)
                    {
                        originX.at<int>(i, offset) = originX.at<int>(i, j);
                        originY.at<int>(i, offset) = originY.at<int>(i, j);
                        offset ++;
                    }
        } else break;
    }
    src.copyTo(dst);

    return seams;
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

vector<string> algorithms;
vector<std::function<void(const Mat&, Mat&)> > funcs;

void testSeamCarving(string testname, string filename, double rows_scale, double cols_scale)
{
    cout << "testSeamCarving : " << testname << endl;

    string path = "../seam_images/scale/tests/" + testname;
    system(("mkdir -p \"" + path + "\"").c_str());
    Mat src = imread("../seam_images/scale/" + filename);
    imwrite(path + "/origin.png", src);

    int output_height = round(src.rows*rows_scale);
    int output_width = round(src.cols*cols_scale);

    for(int i = 0; i < (int)algorithms.size(); i ++)
    {
        Mat dst;
        Mat seams = SeamCarving(src, dst, funcs[i], output_height, output_width);
        imwrite(path + "/" + algorithms[i] + ".png", dst);
        if (output_height <= src.rows && output_width <= src.cols)
        {
            imwrite(path + "/" + algorithms[i] + "-seams.png", seams);
        }
    }
}

int main()
{
    const Mat basic_kernel = (Mat_<int>(1, 3) << -1, 1, 0);
    const Mat sobel_kernel = (Mat_<int>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
    const Mat laplacian_kernel = (Mat_<int>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
    
    std::function<void(const Mat&, Mat&)> basic_func = EOperator(basic_kernel);
    std::function<void(const Mat&, Mat&)> sobel_func = EOperator(sobel_kernel);
    // std::function<void(const Mat&, Mat&)> laplacian_func = [](const Mat &src, Mat &E) {
    //     Mat gray_src;
    //     cvtColor(src, gray_src, CV_BGR2GRAY);
    //     Mat dst;
    //     Laplacian(gray_src, dst, -1);
    //     for(int i = 0; i < src.rows; i ++)
    //         for(int j = 0; j < src.cols; j ++)
    //         {
    //             E.at<int>(i, j) = dst.at<uint8>(i, j);
    //         }
    // };
    std::function<void(const Mat&, Mat&)> laplacian_func = EOperator(laplacian_kernel);
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
    std::function<void(const Mat&, Mat&)> roberts_func = [](const Mat& src, Mat& E) {
        Mat gray_src;
        cvtColor(src, gray_src, CV_BGR2GRAY);
        Mat dx, dy;
        Mat dx_kernel = (Mat_<int>(2, 2) << 1, 0, 0, -1);
        Mat dy_kernel = (Mat_<int>(2, 2) << 0, 1, -1, 0);
        filter2D(gray_src, dx, -1, dx_kernel);
        filter2D(gray_src, dy, -1, dy_kernel);
        for(int i = 0; i < gray_src.rows; i ++)
            for(int j = 0; j < gray_src.cols; j ++)
                E.at<int>(i, j) = min(255, (int)sqrt(double(dx.at<uint8>(i, j))*double(dx.at<uint8>(i, j))+double(dy.at<uint8>(i, j))*double(dy.at<uint8>(i, j))));
    };

    algorithms.push_back("Basic"); funcs.push_back(basic_func);
    algorithms.push_back("Sobel"); funcs.push_back(sobel_func);
    algorithms.push_back("Laplacian"); funcs.push_back(laplacian_func);
    algorithms.push_back("Roberts"); funcs.push_back(roberts_func);

    // testSeamCarving("test1x0.9", "1.jpg", 1, 0.9);
    // testSeamCarving("test1x0.8", "1.jpg", 1, 0.8);
    // testSeamCarving("test1x0.6", "1.jpg", 1, 0.6);
    // testSeamCarving("test1x0.5", "1.jpg", 1, 0.5);
    // testSeamCarving("test1x1.1", "1.jpg", 1, 1.1);

    testSeamCarving("test2(x1x0.9)", "2.png", 1, 0.9);
    testSeamCarving("test2(x0.9)", "2.png", 0.9, 0.9);
    testSeamCarving("test2(x0.6)", "2.png", 0.6, 0.6);

    // testSeamCarving("test3(x1.1)", "3.jpg", 1.1, 1.1);
    // testSeamCarving("test3(x1x1.1)", "3.jpg", 1, 1.1);
    // testSeamCarving("test3(x1.1x1)", "3.jpg", 1.1, 1);

    // testSeamCarving("test4(x1x1.1)", "4.jpg", 1, 1.1);
    // testSeamCarving("test4(x1x1.2)", "4.jpg", 1, 1.2);
    // testSeamCarving("test4(x1x1.3)", "4.jpg", 1, 1.3);

    // testSeamCarving("test5(x1x0.9)", "5.jpg", 1, 0.9);
    // testSeamCarving("test5(x1x0.8)", "5.jpg", 1, 0.8);

    // testSeamCarving("test6(x1x0.9)", "6.jpg", 1, 0.9);
    // testSeamCarving("test6(x1x0.8)", "6.jpg", 1, 0.8);
    // testSeamCarving("test6(x0.9x0.9)", "6.jpg", 0.9, 0.9);

    // testSeamCarving("test7(x1x0.9)", "7.jpg", 1, 0.9);
    // testSeamCarving("test7(x1x0.8)", "7.jpg", 1, 0.8);

    // testSeamCarving("test8(x1x0.8)", "8.jpg", 1, 0.8);

    // testSeamCarving("test9(x1x0.8)", "9.jpg", 1, 0.8);

    // testSeamCarving("test10(x1x1.1)", "9.jpg", 1, 1.1);

    // {
    //     Mat img = imread("../seam_images/remove/1(colored).png", CV_LOAD_IMAGE_COLOR);
    //     Mat dst;
    //     Mat seams = RemoveItem(img, dst, laplacian_func, Vec3b(0, 255, 0));
    //     imwrite("../seam_images/remove/1(removed).png", dst);
    //     imwrite("../seam_images/remove/1(seams).png.png", seams);
    // }

    // {
    //     Mat img = imread("../seam_images/remove/5(colored).png", CV_LOAD_IMAGE_COLOR);
    //     Mat dst;
    //     Mat seams = RemoveItem(img, dst, roberts_func, Vec3b(0, 255, 0));
    //     imwrite("../seam_images/remove/5(removed).png", dst);
    //     imwrite("../seam_images/remove/5(seams).png.png", seams);
    // }

    // {
    //     Mat img = imread("../seam_images/remove/6(colored).png", CV_LOAD_IMAGE_COLOR);
    //     Mat dst;
    //     Mat seams = RemoveItem(img, dst, laplacian_func, Vec3b(0, 255, 0));
    //     imwrite("../seam_images/remove/6(removed).png", dst);
    //     imwrite("../seam_images/remove/6(seams).png.png", seams);
    // }

    return 0;
}