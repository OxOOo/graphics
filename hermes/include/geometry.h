#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "const.h"

// 几何相关

const double EPS  = 1e-6;
const double PI = acos(-1);

struct Point // 点(x, y, z)
{
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {}
};
typedef Point Vector;

struct Line // 线(s+t*d)
{
    Point s;
    Vector d; // better Normalize
    Line(Point s = Point(), Vector d = Vector()) : s(s), d(d) {}
};
typedef Line Ray;

int dcmp(double x); // x<-EPS : -1; -EPS<=x<=EPS : 0; x>EPS : 1

bool operator ==(const Point& A, const Point& B);

Point operator +(const Point& A, const Vector& V);
Vector operator -(const Point& A, const Point& B);
Vector operator *(const Vector& V, double p);
Vector operator /(const Vector& V, double p);
Vector operator -(const Vector& V);

double Dot(const Vector& A, const Vector& B);
double Length(const Vector& V);
double SqrLength(const Vector& V);
Vector Cross(const Vector& A, const Vector& B);
Vector Normalize(const  Vector& V);

double IntersectWithSurface(const Ray& ray, const Vector& n, double D); // 和面求交点

#endif // GEOMETRY_H