#include "grid_material.h"

GridMaterial::GridMaterial(double scale, double reflectiveness)
    : Material(reflectiveness), scale(scale)
{

}

RGB GridMaterial::sample(const Ray& ray, const Point& p, const Vector& n, const vector<Light::ptr>& lights) const
{
    if (lights.size() == 0) return RGB::black();
    return abs((long long)(floor(p.x * scale) + floor(p.y * scale)) % 2) < 1 ? RGB::black() : RGB::white();
}