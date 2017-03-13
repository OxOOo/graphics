#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <image.h>

using namespace std;

int main(int argc, char* argv[])
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    LOG_INFO << "Hello World";

    Image::ptr image = Image::imread(argv[1]);
    LOG_INFO << image->height() << " " << image->width();
    for(int i = 0; i*2 < image->height(); i++)
        for(int j = 0; j < image->width(); j ++)
        {
            image->setPixel(i, j, 255, 0, 0, 255);
        }
    image->save("png2.png");

    return 0;
}