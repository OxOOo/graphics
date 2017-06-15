#ifndef MATERIAL_H
#define MATERIAL_H

#include "const.h"
#include "geometry.h"
#include "rgb.h"
#include "light.h"

// 材质
class Material
{
public:
    const double reflectiveness;
    Material(double reflectiveness): reflectiveness(reflectiveness) {}
    virtual ~Material() {}
    typedef shared_ptr<Material> ptr;

    // 采样
    virtual RGB sample(const Ray& ray, const Point& p, const Vector& n, const vector<Light::ptr>& lights) const = 0;
};

#endif // MATERIAL_H