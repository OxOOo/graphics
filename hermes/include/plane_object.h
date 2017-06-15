#ifndef PLANE_OBJECT_H
#define PLANE_OBJECT_H

#include "object.h"
#include "geometry.h"

class PlaneObject: public Object
{
public:
    PlaneObject(Vector n, double D, Material::ptr material);

    virtual IntersectInfo intersect(const Ray& ray) const;

private:
    Vector n;
    double D;
};

#endif // PLANE_OBJECT_H