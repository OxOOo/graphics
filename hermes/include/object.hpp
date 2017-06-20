#ifndef OBJECT_H
#define OBJECT_H

#include "const.hpp"
#include "geometry.hpp"
#include "material.hpp"

// 物品基类
class Object
{
public:
    Material::ptr material;
    Object() {}
    virtual ~Object() {}
    typedef shared_ptr<Object> ptr;

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual CollideInfo collide(const Ray& ray) const = 0;
    // 内部光线碰撞
    virtual CollideInfo innerCollide(const Ray& ray) const = 0;
    // 光线折射,有折射光线则返回true
    virtual bool refract(const CollideInfo& cinfo, Ray& output_ray) const = 0;
};

// 平面
class PlaneObject: public Object
{
private:
    Vector n;
    double D;
public:
    PlaneObject(Vector n, double D): n(Normalize(n)), D(D) {}

    virtual CollideInfo collide(const Ray& ray) const {
        double t = CollideWithSurface(ray, n, D);
        if (dcmp(t) <= 0) return NoCollide;
        return (CollideInfo){t, ray.s+ray.d*t, n, ray.d};
    }
    virtual CollideInfo innerCollide(const Ray& ray) const {
        return NoCollide;
    }
    virtual bool refract(const CollideInfo& cinfo, Ray& output_ray) const {
        Vector output_v;
        if (!Refract(cinfo.rayd, cinfo.n, material->refract_n, output_v)) return false;
        output_ray = Ray(cinfo.p, output_v);
        return true;
    }
};

// 球形
class SphereObject: public Object
{
private:
    Point center;
    double radius;
public:
    SphereObject(const Point& center, double radius): center(center), radius(radius) {}

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual CollideInfo collide(const Ray& ray) const {
        Vector l = center - ray.s;
        if (dcmp(SqrLength(l)-radius*radius) <= 0) return NoCollide;
        double tp = Dot(l, ray.d);
        double d = SqrLength(l) - tp*tp;
        if (dcmp(d - radius*radius) >= 0) return NoCollide;
        double t = tp - sqrt(radius*radius - d);
        return (CollideInfo){t, ray.s+ray.d*t, Normalize(ray.s+ray.d*t-center), ray.d};
    }
    virtual CollideInfo innerCollide(const Ray& ray) const {
        double tp = Dot(ray.d, center-ray.s);
        Point p = ray.s + ray.d*tp*2;
        return (CollideInfo){tp*2, p, (center-p)/radius, ray.d};
    }
    virtual bool refract(const CollideInfo& cinfo, Ray& output_ray) const {
        Vector output_v1;
        if (!Refract(cinfo.rayd, cinfo.n, material->refract_n, output_v1)) {
            return false;
        }
        double tp = Dot(output_v1, center-cinfo.p);
        Point p = cinfo.p + output_v1*tp*2;
        Vector output_v2;
        if (!Refract(output_v1, (center-p)/radius, 1.0/material->refract_n, output_v2)) {
            return false;
        }
        output_ray = Ray(p, output_v2);
        return true;
    }
};

#endif // OBJECT_H