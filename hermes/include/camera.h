#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"

// 摄像头
class Camera
{
public:
    // fov 视野度数
    Camera(const Point& eye, const Vector& front, const Vector& up, double fov);

    // sx, sy \in [0, 1], sx从左到右，sy从上到下
    Ray generateRay(double sx, double sy) const;

private:
    Point eye;
    Vector front, up, right;
    double fov;
    Vector basex, dx, basey, dy;
};

#endif // CAMERA_H