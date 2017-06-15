#include "phong_material.h"

#include <assert.h>

PhongMaterial::PhongMaterial(const RGB& diffuse, const RGB& specular, double shininess, double reflectiveness)
    : Material(reflectiveness), diffuse(diffuse), specular(specular), shininess(shininess)
{

}

RGB PhongMaterial::sample(const Ray& ray, const Point& p, const Vector& n, const vector<Light::ptr>& lights) const
{
    if (lights.size() == 0) return RGB::black();
    assert(lights.size() == 1);

    Light::Info linfo = lights[0]->light(p);
    Vector L = Normalize(-linfo.v2p);

    double diffuseFactor = Dot(n, L);
    double specularFactor = Dot(Normalize(L - ray.d), n); // FIXME Normalize(ray.d)

    RGB diffuseTerm = diffuse*max(diffuseFactor, 0.0);
    RGB specularTerm = specular*pow(max(specularFactor, 0.0), shininess);

    return linfo.rgb.modulate(diffuseTerm + specularTerm);
    // return linfo.rgb.modulate(diffuseTerm);
}