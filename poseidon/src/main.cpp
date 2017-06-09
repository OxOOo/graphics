#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// BGR
typedef unsigned char uint8;

uint8* SeamCarving(
    const uint8* ptr, int input_width, int input_height,
    const int output_width, const int output_height)
{
    const int MaxW = max(input_width, output_width);
    const int MaxH = max(input_height, output_height);

    uint8 *input_ptr = new uint8[MaxW*MaxH*3];
    uint8 *output_ptr = new uint8[MaxW*MaxH*3];
    memcpy(input_ptr, ptr, sizeof(uint8)*input_width*input_height*3);

    vector<vector<int> > E(MaxH, vector<int>(MaxW, 0));
    vector<vector<int> > DP(MaxH, vector<int>(MaxW, 0));
    vector<vector<int> > PATH(MaxH, vector<int>(MaxW, 0));
    vector<vector<int> > MASK(MaxH, vector<int>(MaxW, 0));

    while(input_width>output_width)
    {
        for(int i = 0; i < input_height; i ++)
            for(int j = 0; j < input_width; j ++)
            {
                int dx = 0, dy = 0;
                for(int k = 0; k < 3; k ++)
                {
                    if (i > 0)
                        dx += abs(*(input_ptr+i*input_width+j*3+k)-*(input_ptr+(i-1)*input_width+j*3+k));
                    if (j > 0)
                        dy += abs(*(input_ptr+i*input_width+j*3+k)-*(input_ptr+i*input_width+(j-1)*3+k));
                }
                E[i][j] = dx+dy;
            }
        for(int i = 0; i < input_height; i ++)
            for(int j = 0; j < input_width; j ++)
            {
                DP[i][j] = E[i][j];
                PATH[i][j] = 0;
                if (i > 0)
                {
                    DP[i][j] = E[i][j] + DP[i-1][j];
                    PATH[i][j] = j;
                    if (j > 0 && DP[i][j] > E[i][j] + DP[i-1][j-1])
                    {
                        DP[i][j] = E[i][j] + DP[i-1][j-1];
                        PATH[i][j] = j-1;
                    }
                    if (j < input_width-1 && DP[i][j] > E[i][j] + DP[i-1][j+1])
                    {
                        DP[i][j] = E[i][j] + DP[i-1][j+1];
                        PATH[i][j] = j+1;
                    }
                }
            }
        int min_e = 0x7fffffff/2, min_j;
        for(int j = 0; j < input_width; j ++)
            if (min_e > DP[input_height-1][j])
            {
                min_e = DP[input_height-1][j];
                min_j = j;
            }
        for(int i = 0; i < input_height; i ++)
            for(int j = 0; j < input_width; j ++)
                MASK[i][j] = 0;
        for(int i = input_height-1; i >= 0; i --)
        {
            MASK[i][min_j] = 1;
            min_j = PATH[i][min_j];
        }
        // =======debug========
        Mat img(input_height, input_width, CV_8UC3);
        for(int i = 0; i < input_height; i ++)
            for(int j = 0; j < input_width; j ++)
            {
                for(int k = 0; k < 3; k ++)
                    *(img.ptr<uint8>()+i*input_width*3+j*3+k) = *(input_ptr+i*input_width*3+j*3+k);
                if (MASK[i][j] == 1)
                {
                    *(img.ptr<uint8>()+i*input_width*3+j*3+0) = 0;
                    *(img.ptr<uint8>()+i*input_width*3+j*3+1) = 0;
                    *(img.ptr<uint8>()+i*input_width*3+j*3+2) = 255;
                }
            }
        char buf[100];
        sprintf(buf, "../seam_images/debug%d.png", input_width);
        imwrite(buf, img);

        for(int i = 0; i < input_height; i ++)
            for(int j = 0, offset = 0; j < input_width; j ++)
                if (!MASK[i][j])
                {
                    for(int k = 0; k < 3; k ++)
                        *(output_ptr+i*(input_width-1)*3+offset*3+k) = *(input_ptr+i*input_width*3+j*3+k);
                    offset ++;
                }
        input_width --;
        memcpy(input_ptr, output_ptr, sizeof(uint8)*input_width*output_width*3);
    }
    delete[] input_ptr;

    return output_ptr;
}

int main()
{
    Mat img = imread("../seam_images/1.jpg", CV_LOAD_IMAGE_COLOR);
    const int input_width = img.cols;
    const int input_height = img.rows;
    const int output_width = img.cols-20;
    const int output_height = img.rows;
    uint8* rst_ptr = SeamCarving(
        img.ptr<uint8>(), input_width, input_height,
        output_width, output_height);
    
    Mat M(output_height, output_width, CV_8UC3);
    memcpy(M.ptr<uint8>(), rst_ptr, sizeof(uint8)*output_width*output_height*3);
    imwrite("../seam_images/temp.png", M);

    cout << "============" << endl;

    delete[] rst_ptr;

    return 0;
}