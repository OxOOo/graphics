#include "plane_object.h"

PlaneObject::PlaneObject(Vector n, double D, Material::ptr material)
    : Object(material)
{
    this->n = Normalize(n);
    this->D = D;
}

Object::IntersectInfo PlaneObject::intersect(const Ray& ray) const
{
    double t = IntersectWithSurface(ray, n, D);
    if (dcmp(t) <= 0) return NoIntersect;
    return (IntersectInfo){t, ray.s+ray.d*t, n};
}