#ifndef MATERIAL_H
#define MATERIAL_H

#include "const.hpp"
#include "geometry.hpp"
#include "rgb.hpp"
#include "light.hpp"
#include <assert.h>

// 材质
class Material
{
public:
    double reflect_factor; // 反射比例
    double refract_factor; // 折射比例
    double diffuse_factor; // 漫反射比例
    double refract_n; // 折射率
    double high_light; // 高光系数，phong模型
    RGB absorb_color;

    Material() {}
    virtual ~Material() {}
    typedef shared_ptr<Material> ptr;

    // 采样
    virtual RGB plaincolor(const CollideInfo& cinfo) const = 0;

    // 直接光照，考虑diffuse_factor和high_light
    virtual RGB sample(const LightInfo& linfo, const CollideInfo& cinfo) const
    {
        Vector L = -linfo.light.d;
        RGB color = plaincolor(cinfo);

        double diffuseFactor = Dot(cinfo.n, L);
        double specularFactor = Dot(Normalize(L - cinfo.rayd), cinfo.n);

        RGB diffuseTerm = linfo.rgb.modulate(color*max(diffuseFactor, 0.0)*diffuse_factor);
        RGB specularTerm = linfo.rgb*pow(max(specularFactor, 0.0), 20)*high_light;

        return diffuseTerm + specularTerm;
    }
};

// 黑白网格
class GridMaterial: public Material
{
private:
    double scale;
public:
    GridMaterial(double scale): scale(scale) {}

    virtual RGB plaincolor(const CollideInfo& cinfo) const {
        return abs((long long)(floor(cinfo.p.x * scale) + floor(cinfo.p.y * scale)) % 2) < 1 ? RGB::black() : RGB::white();
    }
};

// 单色模型
class PlainMaterial : public Material
{
private:
    RGB color;
public:
    PlainMaterial(const RGB& color): color(color) {}

    virtual RGB plaincolor(const CollideInfo& cinfo) const {
        return color;
    }
};

#endif // MATERIAL_H