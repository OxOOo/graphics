#ifndef LIGHT_H
#define LIGHT_H

#include "const.hpp"
#include "rgb.hpp"
#include "geometry.hpp"

// 光照
class Light
{
public:
    RGB color;
    Light() {}
    virtual ~Light() {}
    typedef shared_ptr<Light> ptr;

    // 光子映射生成光线
    virtual vector<LightInfo> genLights() const = 0;
    // 直接光照
    virtual vector<LightInfo> targetLights(const Point& target) const = 0;
    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual CollideInfo collide(const Ray& ray) const = 0;
};

// 锥光源
class ConeLight: public Light
{
private:
    Point p;
    Vector v;
    double fov, cos_limit;
public:
    // v: 光源的朝向方向
    ConeLight(const Point& p, const Vector& v, double fov)
        : p(p), v(Normalize(v))
    {
        this->fov = fov*PI/180.0;
        this->cos_limit = cos(this->fov/2);
    }

    virtual vector<LightInfo> genLights() const
    {
        vector<LightInfo> lights;
        return lights;
    }
    virtual vector<LightInfo> targetLights(const Point& target) const
    {
        vector<LightInfo> lights;
        Vector v2p = Normalize(target-p);
        double cos_angle = Dot(v2p, v);
        if (dcmp(cos_angle) > 0 && dcmp(cos_angle-cos_limit)>=0)
            lights.push_back((LightInfo){color, Ray(p, v2p)});
        else lights.push_back((LightInfo){RGB::black(), Ray(p, v2p)});
        return lights;
    }
    virtual CollideInfo collide(const Ray& ray) const
    {
        return NoCollide;
    }
};

// // 锥光源
// class ConeLight: public Light
// {
// private:
//     Point p;
//     Vector v;
//     double r_source, r_lid;
//     double D1, D2;
//     Vector x, y;
// public:
//     // v:光线来的方向
//     ConeLight(const Point& p, const Vector& v, double r_source, double r_lid)
//         : p(p), v(Normalize(v)), r_source(r_source), r_lid(r_lid)
//     {
//         D1 = -Dot(p, v);
//         D2 = -Dot(p+v, v);
//         x = Cross(v, Vector(1, 0, 0));
//         y = Cross(v, x);
//     }

//     virtual vector<LightInfo> genLights() const
//     {
//         vector<LightInfo> lights;
//         for(int i = 0; i < 10; i ++) { // FIXME
//             double phi1 = RAND()*PI*2;
//             double r1 = sqrt(RAND())*r_source;
//             double phi2 = RAND()*PI*2;
//             double r2 = sqrt(RAND())*r_source;
//             Point p1 = p+(cos(phi1)*x+sin(phi1)*y)*r1;
//             Point p2 = p+(cos(phi2)*x+sin(phi2)*y)*r2 + v;
//             lights.push_back((LightInfo){color, Ray(p1, Normalize(p2-p1))});
//         }
//         return lights;
//     }
//     virtual vector<LightInfo> targetLights(const Point& target) const
//     {
//         vector<LightInfo> lights;
//         for(int i = 0; i < 20; i ++) {
//             double phi1 = RAND()*PI*2;
//             double r1 = sqrt(RAND())*r_source;
//             Point p1 = p+(cos(phi1)*x+sin(phi1)*y)*r1;
//             Vector v = Normalize(target-p1);
//             Ray ray(p1, v);
//             double t = CollideWithSurface(ray, v, D2);
//             if (dcmp(t) > 0 && dcmp(Length(ray.s+ray.d*t-(p+v)) - r_lid) <= 0)
//                 lights.push_back((LightInfo){color, Ray(p1, v)});
//             else lights.push_back((LightInfo){RGB::black(), Ray(p1, v)});
//         }
//         return lights;
//     }
//     virtual CollideInfo collide(const Ray& ray) const
//     {
//         double t1 = CollideWithSurface(ray, v, D1);
//         if (dcmp(t1) > 0 && dcmp(Length(ray.s+ray.d*t1-p)-r_source) <= 0)
//             return (CollideInfo){t1, ray.s+ray.d*t1, v, ray.d};
//         return NoCollide;
//     }
// };

#endif // LIGHT_H