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
    virtual vector<Ray> generateRay(int h, int w) const = 0;
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
public:
    // fov 视野度数
    PerspectiveCamera(const Point& eye, const Vector& front, const Vector& up, double fov, int size)
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
    virtual vector<Ray> generateRay(int h, int w) const
    {
        vector<Ray> rays;
        rays.push_back(Ray(eye, Normalize(front+basex+dx*(w+0.5)/size+basey+dy*(h+0.5)/size)));
        return rays;
    }
};
