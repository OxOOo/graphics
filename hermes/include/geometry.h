#ifndef GEOMETRY_H
#define GEOMETRY_H

// 几何相关

const double EPS  = 1e-6;

struct Point // 点(x, y, z)
{
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {}
};
typedef Point Vector;

struct Line // 线(s+t*d)
{
    Point s;
    Vector d;
    Line(Point s = Point(), Vector d = Vector()) : s(s), d(d) {}
};
typedef Line Ray;

/*
 * ltpoint : 左上角
 * lbpoint : 左下角
 * rtpoint : 右上角
 */
struct Rect
{
    Point ltpoint, lbpoint, rtpoint;
    Rect(Point ltpoint = Point(), Point lbpoint = Point(), Point rtpoint = Point()) : ltpoint(ltpoint), lbpoint(lbpoint), rtpoint(rtpoint) {}
};

int dcmp(double x); // x<-EPS : -1; -EPS<=x<=EPS : 0; x>EPS : 1

bool operator ==(const Point& A, const Point& B);

Point operator +(const Point& A, const Vector& V);
Vector operator -(const Point& A, const Point& B);
Vector operator *(const Vector& V, double p);
Vector operator /(const Vector& V, double p);

double Dot(const Vector& A, const Vector& B);
double Length(const Vector& V);
Vector Cross(const Vector&A, const Vector& B);

double ImpactWithSurface(const Ray& ray, const Vector& n, const Vector& D); // 和面求交点

#endif // GEOMETRY_H