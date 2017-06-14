#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "time_log.h"
#include "scene.h"

using namespace std;

int main()
{
    setTimePoint("PROCESS");

    cout << "Hello World" << endl;

    Scene *scene = new Scene();
    cv::Mat img = scene->render(Point(0, -5, 0), Rect(Point(-5, -3, 5), Point(-5, -3, -5), Point(5, -3, 5)), 1024, 1024);
    cv::imwrite("temp.png", img);
    delete scene;

    logTimePoint("PROCESS");
    return 0;
}