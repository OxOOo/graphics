#include "camera.h"

// fov 视野度数
Camera::Camera(const Point& eye, const Vector& front, const Vector& up, double fov)
    : eye(eye), front(Normalize(front)), up(Normalize(up)), fov(fov)
{
    right = Cross(front, up);
    cout << "right : " << right.x << " " << right.y << " " << right.z << endl;
    double length = 2*tan(fov*0.5*PI/180)*1;
    basex = -right*length/2;
    dx = right*length;
    basey = up*length/2;
    dy = -up*length;
}

// sx, sy \in [0, 1]
Ray Camera::generateRay(double sx, double sy) const
{
    return Ray(eye, Normalize(front+basex+dx*sx+basey+dy*sy));
}