#ifndef GEOMETRY_H
#define GEOMETRY_H

// 几何相关

struct Point // 点(x, y, z)
{
    double x, y, z;
};
typedef Point Vector;

struct Line // 线(s+t*d)
{
    Point s;
    Vector d;
};
typedef Line Ray;

#endif // GEOMETRY_H