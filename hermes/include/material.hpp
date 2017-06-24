#ifndef MATERIAL_H
#define MATERIAL_H

#include "const.hpp"
#include "geometry.hpp"
#include "rgb.hpp"
#include "light.hpp"
#include <assert.h>
#include <opencv2/opencv.hpp>

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
    cv::Mat texture; // 纹理

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

    double BRDF(const Vector& ray_in, const Vector& n, const Vector& ray_out)
    {
        // double ret = 0;
        // ret += max(diffuse_factor * Dot(-ray_in, n), 0.0);
        // ret += high_light * pow(max(Dot(-ray_in, Reflect(ray_out, n)), 0.0), 20);
        // return ret;
        return max(diffuse_factor * Dot(-ray_in, n), 0.0);
    }

    bool hasTexture() const {
        return texture.rows > 0;
    }

    RGB getSmoothTexture(double u , double v ) const
    {
        double U = ( u - floor( u ) ) * texture.rows;
        double V = ( v - floor( v ) ) * texture.cols;
        int U1 = ( int ) floor( U - 1e-3 ) , U2 = U1 + 1;
        int V1 = ( int ) floor( V - 1e-3 ) , V2 = V1 + 1;
        double rat_U = U2 - U;
        double rat_V = V2 - V;
        if ( U1 < 0 ) U1 = texture.rows - 1; if ( U2 == texture.rows ) U2 = 0;
        if ( V1 < 0 ) V1 = texture.cols - 1; if ( V2 == texture.cols ) V2 = 0;
        RGB ret;
        ret = ret + RGB(texture.at<cv::Vec3b>(U1, V1)) * rat_U * rat_V;
        ret = ret + RGB(texture.at<cv::Vec3b>(U1, V2)) * rat_U * ( 1 - rat_V );
        ret = ret + RGB(texture.at<cv::Vec3b>(U2, V1)) * ( 1 - rat_U ) * rat_V;
        ret = ret + RGB(texture.at<cv::Vec3b>(U2, V2)) * ( 1 - rat_U ) * ( 1 - rat_V );
        return ret;
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