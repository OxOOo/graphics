#pragma once

#include "geometry.hpp"
#include "const.hpp"

// 摄像头
class Camera
{
public:
    Camera() {}
    virtual ~Camera() {}
    typedef shared_ptr<Camera> ptr;

    virtual int getHeight() const = 0;
    virtual int getWidth() const = 0;
    // h \in 0~height-1
    virtual bool needSmooth() const = 0;
    virtual vector<Ray> generateRay(int h, int w) const = 0;
    virtual vector<Ray> generateSmoothRay(int h, int w) const = 0;
};

// 透视摄像机
class PerspectiveCamera : public Camera
{
private:
    Point eye;
    Vector front, up, right;
    double fov;
    Vector basex, dx, basey, dy;
    int size;
    int smooth_num;
public:
    // fov 视野度数
    PerspectiveCamera(const Point& eye, const Vector& front, const Vector& up, double fov, int size, int smooth_num)
        : eye(eye), front(Normalize(front)), up(Normalize(up)), fov(fov), size(size), smooth_num(smooth_num)
    {
        right = Cross(front, up);
        double length = 2*tan(fov*0.5*PI/180)*1;
        basex = -right*length/2;
        dx = right*length;
        basey = up*length/2;
        dy = -up*length;
    }

    virtual int getHeight() const { return size; }
    virtual int getWidth() const { return size; }
    virtual bool needSmooth() const {return true;}
    virtual vector<Ray> generateRay(int h, int w) const
    {
        vector<Ray> rays;
        double x = (w+0.5)/size;
        double y = (h+0.5)/size;
        rays.push_back(Ray(eye, Normalize(front+basex+dx*x+basey+dy*y)));
        return rays;
    }
    virtual vector<Ray> generateSmoothRay(int h, int w) const
    {
        vector<Ray> rays;
        for(int i = 0; i < smooth_num; i ++)
            for(int j = 0; j < smooth_num; j ++)
            {
                double x = (w+0.5/smooth_num+1.0/smooth_num*i)/size;
                double y = (h+0.5/smooth_num+1.0/smooth_num*j)/size;
                rays.push_back(Ray(eye, Normalize(front+basex+dx*x+basey+dy*y)));
            }
        return rays;
    }
};

// 景深摄像机
class DeepCamera : public Camera
{
private:
    Point eye;
    Vector front, up, right;
    double fov;
    int size;
    Vector basex, dx, basey, dy;
public:
    double lens_R, lens_dis, item_dis;
    int sample_num;
public:
    // fov 视野度数
    DeepCamera(const Point& eye, const Vector& front, const Vector& up, double fov, int size)
        : eye(eye), front(Normalize(front)), up(Normalize(up)), fov(fov), size(size)
    {
        right = Cross(front, up);
        double length = 2*tan(fov*0.5*PI/180)*1;
        basex = -right*length/2;
        dx = right*length;
        basey = up*length/2;
        dy = -up*length;
    }

    virtual int getHeight() const { return size; }
    virtual int getWidth() const { return size; }
    virtual bool needSmooth() const {return false;}
    virtual vector<Ray> generateRay(int h, int w) const
    {
        vector<Ray> rays;
        double x = (w+0.5)/size;
        double y = (h+0.5)/size;
        Vector v = Normalize(front+basex+dx*x+basey+dy*y);
        Vector target = eye + Dot(v, front) * item_dis * v;
        for(int i = 0; i < sample_num; i ++)
        {
            double theta = RAND()*PI*2;
            double r = RAND()*RAND()*lens_R;
            Vector o = eye + front*lens_dis + r*up*cos(theta) + r*right*sin(theta);
            rays.push_back(Ray(o, Normalize(target-o)));
        }
        return rays;
    }
    virtual vector<Ray> generateSmoothRay(int h, int w) const
    {
        return vector<Ray>();
    }
};
