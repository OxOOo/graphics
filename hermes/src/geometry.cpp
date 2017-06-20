#include "geometry.hpp"

#include <cmath>
#include <iostream>
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
Vector operator *(double p, const Vector& V)
{
    return Vector(V.x*p, V.y*p, V.z*p);
}
Vector operator /(const Vector& V, double p)
{
    return Vector(V.x/p, V.y/p, V.z/p);
}
Vector operator -(const Vector& V)
{
    return Vector(-V.x, -V.y, -V.z);
}

double Dot(const Vector& A, const Vector& B)
{
    return A.x*B.x+A.y*B.y+A.z*B.z;
}
double Length(const Vector& V)
{
    return sqrt(V.x*V.x+V.y*V.y+V.z*V.z);
}
double SqrLength(const Vector& V)
{
    return V.x*V.x+V.y*V.y+V.z*V.z;
}
Vector Cross(const Vector& A, const Vector& B)
{
    return Vector(A.y*B.z-A.z*B.y, A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x);
}
Vector Normalize(const  Vector& V)
{
    double l = Length(V);
    if (dcmp(l) == 0) return Vector(0, 0, 0);
    return V/l;
}

double CollideWithSurface(const Ray& ray, const Vector& n, double D) // 和面求交点
{
    // t = -|D+n*ray.s|/|n*ray.d|
    double b = Dot(n, ray.d);
    if (dcmp(b) == 0) return -1;
    return -(D+Dot(n, ray.s))/b;
}

Vector Reflect(const Vector& input_v, const Vector& n)
{
    return n* (-2 * Dot(n, input_v)) + input_v;
}

bool Refract(const Vector& input_v, const Vector& n, double refract_n, Vector& output_v)
{
    refract_n = 1.0 / refract_n; // 输入refract_n = 物体相对真空的折射率
	double cosI = -Dot(input_v, n);
    double cosT2 = 1 - refract_n * refract_n * ( 1 - cosI * cosI );
	if ( cosT2 > EPS ) {
        output_v = Normalize(input_v * refract_n + n * ( refract_n * cosI - sqrt( cosT2 ) ) );
        return true;
    }
    
    return false;
}