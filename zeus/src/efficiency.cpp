#include <efficiency.h>
#include <plog/Log.h>

#include <map>
#include <ctime>

using namespace std;

map<string, clock_t> time_points;

void timePointStart(const std::string& name)
{
    if(time_points.find(name) != time_points.end())
    {
        LOG_ERROR << "time point(" << name << ") has already exists";
    }
    time_points[name] = clock();
}

void timePointEnd(const std::string& name)
{
    if(time_points.find(name) == time_points.end())
    {
        LOG_FATAL << "time point(" << name << ") has not exists";
        return;
    }
    double time_cost = double(clock() - time_points[name]) / CLOCKS_PER_SEC;
    time_points.erase(time_points.find(name));
    LOG_INFO << "time cost(" << name << ") : " << time_cost << " s";
}