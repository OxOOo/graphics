#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

// 几何相关

const double EPS  = 1e-6;
const double PI = std::acos(-1);
const double INF = 1e10;

struct Point // 点(x, y, z)
{
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {}
    const double& operator[](int axis) const {if(axis==0)return x;else if(axis==1)return y;else return z;}
    double& operator[](int axis) {if(axis==0)return x;else if(axis==1)return y;else return z;}
    void print() const;
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
Vector operator *(double p, const Vector& V);
Vector operator /(const Vector& V, double p);
Vector operator -(const Vector& V);

double Dot(const Vector& A, const Vector& B);
double Length(const Vector& V);
double Length2(const Vector& V);
Vector Cross(const Vector& A, const Vector& B);
Vector Normalize(const Vector& V);
Vector GetVerticalVector(const Vector& V); // 返回一个与给定向量垂直的向量
Vector GetRandomVector(const Vector& N);

// 和面求交点, Normalized
double CollideWithSurface(const Ray& ray, const Vector& n, double D);
// 镜像
Vector Reflect(const Vector& input_v, const Vector& n);
// 折射，折射成功返回true, Normalized, refract_n = 物体相对真空的折射率
bool Refract(const Vector& input_v, const Vector& n, double refract_n, Vector& output_v);

#endif // GEOMETRY_H