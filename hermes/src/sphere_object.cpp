#include "sphere_object.h"

SphereObject::SphereObject(const Point& center, double radius, Material::ptr material)
{
    this->center = center;
    this->radius = radius;
    this->material = material;
}

// 渲染，返回RGB
RGB SphereObject::render(const Ray& ray, double t, const Scene* scene, int deep, const vector<Light::ptr>& possible_ligts) const
{
    RGB mrgb = material->sample(ray, t, Normalize(ray.s+ray.d*t-center), possible_ligts);
    return mrgb;
}

// 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
double SphereObject::intersect(const Ray& ray) const
{
    Vector l = center - ray.s;
    if (dcmp(SqrLength(l)-radius*radius) <= 0) return -1;
    double tp = Dot(l, ray.d);
    double d = SqrLength(l) - tp*tp;
    if (dcmp(d - radius) >= 0) return -1;
    double t = tp - sqrt(radius*radius - d*d);
    return t;
}