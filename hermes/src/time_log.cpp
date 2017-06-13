#include "time_log.h"
#include <map>
#include <ctime>
#include <iostream>
#include <sys/time.h>

using namespace std;

map<string, double> clock_points;

void setTimePoint(string tag)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    clock_points[tag] = tv.tv_sec + 1e-6*tv.tv_usec;
}

double logTimePoint(string tag)
{

    if (clock_points.find(tag) != clock_points.end()) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        double now = tv.tv_sec + 1e-6*tv.tv_usec;
        cout << "耗时(" << tag << "):" << double(now-clock_points[tag]) << " s" << endl;
        return double(now-clock_points[tag]);
    } else {
        cout << "耗时(" << tag << "):未知" << endl;
        return 0.0;
    }
}