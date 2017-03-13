#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <image.h>
#include <panel.h>
#include <efficiency.h>

using namespace std;

const double PI = acos(-1);

void imageTest()
{
    Image::ptr image = Image::imread("../pngtest.png");
    LOG_INFO << image->height() << " " << image->width();
    for(int i = 0; i*2 < image->height(); i++)
        for(int j = 0; j < image->width(); j ++)
        {
            image->setPixel(i, j, 255, 0, 0, 255);
        }
    image->save("png2.png");
}

int main(int argc, char* argv[])
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    LOG_INFO << "Hello World";
    LOG_INFO << "PI : " << PI;

    Panel panel(600, 800);

    {
        const int x = 300, y = 400, r = 100;
        const int n = 36;

        LOG_INFO << "start to draw line circle";
        timePointStart("drawLine");

        for(int i=0;i<n;i++)
        {
            panel.drawLine(x, y, x+r*cos(2*PI*i/n), y+r*sin(2*PI*i/n), PIXEL_RED);
        }
        
        timePointEnd("drawLine");
    }

    panel.save("rst.png");

    LOG_DEBUG << "ZEUS END";

    return 0;
}