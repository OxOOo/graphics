#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <image.h>
#include <panel.h>
#include <efficiency.h>

using namespace std;

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

    const bool AA = true;
    Panel panel(600, 800, AA);

    {
        const int x = 300, y = 400, r = 100;
        const int n = 36;

        LOG_INFO << "start to draw line";
        timePointStart("drawLine");

        for(int i=0;i<n;i++)
        {
            panel.drawLine(x, y, x+r*cos(2*PI*i/n), y+r*sin(2*PI*i/n), PIXEL_RED);
        }
        
        timePointEnd("drawLine");
    }

    {
        LOG_INFO << "start to draw circle";
        timePointStart("drawCircle");

        panel.drawCircle(200, 200, 75, PIXEL_GREEN);
        
        timePointEnd("drawCircle");
    }

    {
        LOG_INFO << "start to draw polygon1";
        timePointStart("drawPolygon1");

        vector<Pointi> points;
        points.push_back(Pointi(200, 600));
        points.push_back(Pointi(200, 700));
        points.push_back(Pointi(100, 750));
        points.push_back(Pointi(150, 680));
        points.push_back(Pointi(100, 620));

        panel.drawPolygon(points, PIXEL_BLUE);
        
        timePointEnd("drawPolygon1");
    }

    {
        LOG_INFO << "start to draw polygon2";
        timePointStart("drawPolygon2");

        const int x = 450, y = 600, r = 85;

        vector<Pointi> points;
        points.push_back(Pointi(x, y) + Pointi(cos(2*PI*1/5)*r, sin(2*PI*1/5)*r));
        points.push_back(Pointi(x, y) + Pointi(cos(2*PI*3/5)*r, sin(2*PI*3/5)*r));
        points.push_back(Pointi(x, y) + Pointi(cos(2*PI*0/5)*r, sin(2*PI*0/5)*r));
        points.push_back(Pointi(x, y) + Pointi(cos(2*PI*2/5)*r, sin(2*PI*2/5)*r));
        points.push_back(Pointi(x, y) + Pointi(cos(2*PI*4/5)*r, sin(2*PI*4/5)*r));

        panel.drawPolygon(points, PIXEL_YELLOW);
        
        timePointEnd("drawPolygon2");
    }

    {
        LOG_INFO << "start to draw polygon3";
        timePointStart("drawPolygon3");

        vector<Pointi> points;
        points.push_back(Pointi(450, 100));
        points.push_back(Pointi(550, 100));
        points.push_back(Pointi(580, 200));
        points.push_back(Pointi(580, 150));

        panel.drawPolygon(points, PIXEL_BLACK);
        
        timePointEnd("drawPolygon3");
    }

    panel.save(AA ? "rstAA.png" : "rst.png");

    LOG_DEBUG << "ZEUS END";

    return 0;
}