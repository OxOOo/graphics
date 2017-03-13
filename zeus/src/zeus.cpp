#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

using namespace std;

int main(int argc, char* argv[])
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    LOG_INFO << "Hello World";

    return 0;
}