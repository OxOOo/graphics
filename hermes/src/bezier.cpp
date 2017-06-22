#include "bezier.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <assert.h>

using namespace std;
using namespace Eigen;

void Bezier::append(const Point& p)
{
    assert(dcmp(p.y) == 0);
    points.push_back(p);

    double zmin = p.z, zmax = p.z;
    for(int i = 0; i < (int)points.size(); i ++)
    {
        zmin = min(zmin, points[i].z);
        zmax = max(zmax, points[i].z);
    }
    box_center = Point(0, 0, (zmax+zmin)/2);
    box_radius2 = 0;
    for(int i = 0; i < (int)points.size(); i ++)
        box_radius2 = max(box_radius2, Length2(points[i]-box_center));
    cout << "==========Bezier===========" << endl;
    box_center.print();
    cout << box_radius2 << endl;
}

Point Bezier::P(double t) const
{
    DMatrix<Point> mat(points.size(), points.size());
    for(int i = 0; i < (int)points.size(); i ++)
        mat[points.size()-1][i] = points[i];
    for(int i = points.size()-2; i >= 0; i --)
        for(int j = 0; j <= i; j ++)
            mat[i][j] = mat[i+1][j] + (mat[i+1][j+1]-mat[i+1][j])*t;
    return mat[0][0];
}

Vector Bezier::Pt(double t) const
{
    assert(points.size() == 3);
    // B(t) = (1-t)^2*P0+2*t*(1-t)*P1+t^2*P2
    return -2*(1-t)*points[0]+2*(1-2*t)*points[1]+2*t*points[2];
}

cv::Mat Bezier::draw(int size) const
{
    cv::Mat img(size, size, CV_8UC3, cv::Scalar(255, 255, 255));
    for(double t = 0; t < 1.0; t += 0.5/size)
    {
        Point p = P(t);
        int x = -p.z*size/2+size/2;
        int y = p.x*size/2+size/2;
        img.at<cv::Vec3b>(x, y) = cv::Vec3b(255, 0, 0);
    }
    return img;
}

CollideInfo Bezier::collide(const Ray& ray) const
{
    Vector l = box_center - ray.s;
    double tp = Dot(l, ray.d);
    double d2 = Length2(l) - tp*tp;
    if (dcmp(d2 - box_radius2) > 0) return NoCollide;

    auto F = [&ray, this](const Matrix<double,3,1>& in) {
        double x = in[0], theta = in[1], t = in[2];
        Point p = P(x);
        Matrix<double,3,1> ret;
        ret[0] = p.x*cos(theta)-ray.s.x-ray.d.x*t;
        ret[1] = p.x*sin(theta)-ray.s.y-ray.d.y*t;
        ret[2] = p.z-ray.s.z-ray.d.z*t;
        return ret;
    };
    auto Ft = [&ray, this](const Matrix<double,3,1>& in) {
        double x = in[0], theta = in[1]; // t = in[2];
        Point p = P(x);
        Vector pt = Pt(x);
        Matrix<double,3,3> ret;
        ret(0,0)=pt.x*cos(theta); ret(0,1)=-p.x*sin(theta); ret(0,2)=-ray.d.x;
        ret(1,0)=pt.x*sin(theta); ret(1,1)=p.y*cos(theta); ret(1,2)=-ray.d.y;
        ret(2,0)=pt.z; ret(2,1)=0; ret(2,2)=-ray.d.z;
        return ret;
    };

    Matrix<double,3,1> solution;
    double R2 = 1e10, dis = 1e10;

    double Len = Length(ray.s);
    for(int times = 8; times && dcmp(R2) > 0; times --)
    {
        Matrix<double,3,1> x;
        x[0] = RAND();
        x[1] = RAND()*PI*2;
        x[2] = (times-2)*Len/8.0;

        for(int iter = 7; iter && dcmp(R2) > 0; iter --)
        {
            auto tmp = F(x);
            if (dcmp(x[0])>=0 && dcmp(x[0]-1)<=0 && dcmp(x[2])>=0 && dcmp(x[2]-dis)<=0 && dcmp(tmp[0]*tmp[0]+tmp[1]*tmp[1]+tmp[2]*tmp[2])<=0)
            {
                R2 = tmp[0]*tmp[0]+tmp[1]*tmp[1]+tmp[2]*tmp[2];
                solution = x;
                dis = x[2];
            }
            x = x-Ft(x).inverse()*tmp;
        }
    }

    // cout << "R2 : " << R2 << endl;
    if (dcmp(R2) > 0) return NoCollide;
    Point p = P(solution[0]);
    Vector pt = Pt(solution[0]);

    Vector n(pt.z*cos(solution[1]), pt.z*sin(solution[1]), -pt.x);
    Point target(p.x*cos(solution[1]), p.x*sin(solution[1]), p.z);
    double t = Length(target-ray.s);
    return (CollideInfo){t, target, Normalize(n), ray.d};
}
