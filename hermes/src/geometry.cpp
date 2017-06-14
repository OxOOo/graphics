#include "geometry.h"

#include <cmath>
using namespace std;

int dcmp(double x) // x<-EPS : -1; -EPS<=x<=EPS : 0; x>EPS : 1
{
    if (-EPS < x && x < EPS) return 0;
    return x < 0 ? -1 : 1;
}

bool operator ==(const Point& A, const Point& B)
{
    return dcmp(A.x-B.x) == 0 && dcmp(A.y-B.y) == 0 && dcmp(A.z-B.z) == 0;
}

Point operator +(const Point& A, const Vector& V)
{
    return Point(A.x+V.x, A.y+V.y, A.z+V.z);
}
Vector operator -(const Point& A, const Point& B)
{
    return Vector(A.x-B.x, A.y-B.y, A.z-B.z);
}
Vector operator *(const Vector& V, double p)
{
    return Vector(V.x*p, V.y*p, V.z*p);
}
Vector operator /(const Vector& V, double p)
{
    return Vector(V.x/p, V.y/p, V.z/p);
}

double Dot(const Vector& A, const Vector& B)
{
    return A.x*B.x+A.y*B.y+A.z*B.z;
}
double Length(const Vector& V)
{
    return sqrt(V.x*V.x+V.y*V.y+V.z*V.z);
}
Vector Cross(const Vector&A, const Vector& B)
{
    return Vector(A.y*B.z-A.z*B.y, A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x);
}

double ImpactWithSurface(const Ray& ray, const Vector& n, const Vector& D) // 和面求交点
{
    // t = -|D+n*ray.s|/|n*ray.d|
    double b = Length(Cross(n, ray.d));
    if (dcmp(b) == 0) return -1;
    return -Length(D+Cross(n, ray.s))/b;
}
