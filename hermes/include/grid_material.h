#ifndef GRID_MATERIAL_H
#define GRID_MATERIAL_H

#include "material.h"

class GridMaterial: public Material
{
public:
    GridMaterial(double scale, double reflectiveness = 0);

    virtual RGB sample(const Ray& ray, const Point& p, const Vector& n, const vector<Light::ptr>& lights) const;

private:
    double scale;
};

#endif // GRID_MATERIAL_H