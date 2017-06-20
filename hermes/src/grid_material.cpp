#include "grid_material.h"
#include <assert.h>

GridMaterial::GridMaterial(double scale, double reflectiveness)
    : Material(reflectiveness), scale(scale)
{

}

RGB GridMaterial::sample(const Ray& ray, const Point& p, const Vector& n, const vector<Light::ptr>& lights) const
{
    if (lights.size() == 0) return RGB::black();
    assert(lights.size() == 1);

    Light::Info linfo = lights[0]->light(p);
    Vector L = Normalize(-linfo.v2p);

    double diffuseFactor = Dot(n, L);

    RGB color = abs((long long)(floor(p.x * scale) + floor(p.y * scale)) % 2) < 1 ? RGB::black() : RGB::white();
    return linfo.rgb.modulate(color*max(diffuseFactor, 0.0));
}