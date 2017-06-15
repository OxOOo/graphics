#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include "material.h"
#include "geometry.h"

// Phong 模型
class PhongMaterial : public Material
{
public:
    // 漫反射颜色，镜面反射颜色，闪光度
    PhongMaterial(const RGB& diffuse, const RGB& specular, double shininess, double reflectiveness = 0);

    virtual RGB sample(const Ray& ray, const Point& p, const Vector& n, const vector<Light::ptr>& lights) const;

private:
    RGB diffuse;
	RGB specular;
	double shininess;
};

#endif // PHONG_MATERIAL_H