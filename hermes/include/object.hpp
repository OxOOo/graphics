#ifndef OBJECT_H
#define OBJECT_H

#include "const.hpp"
#include "geometry.hpp"
#include "material.hpp"
#include "bezier.hpp"
#include "kdtree.hpp"

// 物品基类
class Object
{
public:
    Material::ptr material;
    unsigned long long sample;
    Object() {sample = rand()%731;}
    virtual ~Object() {}
    typedef shared_ptr<Object> ptr;

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual CollideInfo collide(const Ray& ray) const = 0;
    // 内部光线碰撞
    virtual CollideInfo innerCollide(const Ray& ray) const = 0;
    // 获取纹理
    virtual RGB getTexture(const CollideInfo& cinfo) const = 0;
};

// 平面
class PlaneObject: public Object
{
private:
    Vector n;
    double D;
    Vector dx, dy;
    double texture_scale;
public:
    PlaneObject(Vector n, double D, double texture_scale = 1): n(Normalize(n)), D(D), texture_scale(texture_scale) {
        dx = GetVerticalVector(n);
        dy = Cross(n, dx);
        dx = Normalize(dx)*texture_scale;
        dy = Normalize(dy)*texture_scale;
    }

    virtual CollideInfo collide(const Ray& ray) const {
        double t = CollideWithSurface(ray, n, D);
        if (dcmp(t) <= 0) return NoCollide;
        return (CollideInfo){t, ray.s+ray.d*t, n, ray.d};
    }
    virtual CollideInfo innerCollide(const Ray& ray) const {
        return NoCollide;
    }
    virtual RGB getTexture(const CollideInfo& cinfo) const {
        double u = Dot( dx, cinfo.p ) / texture_scale;
        double v = Dot( dy, cinfo.p ) / texture_scale;
        return material->getSmoothTexture( u , v );
    }
};

// 球形
class SphereObject: public Object
{
private:
    Point center;
    double radius;
public:
    SphereObject(const Point& center, double radius): center(center), radius(radius) {}

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual CollideInfo collide(const Ray& ray) const {
        Vector l = center - ray.s;
        if (dcmp(Length2(l)-radius*radius) <= 0) return NoCollide;
        double tp = Dot(l, ray.d);
        double d2 = Length2(l) - tp*tp;
        if (dcmp(d2 - radius*radius) >= 0) return NoCollide;
        double t = tp - sqrt(radius*radius - d2);
        return (CollideInfo){t, ray.s+ray.d*t, Normalize(ray.s+ray.d*t-center), ray.d};
    }
    virtual CollideInfo innerCollide(const Ray& ray) const {
        double tp = Dot(ray.d, center-ray.s);
        Point p = ray.s + ray.d*tp*2;
        return (CollideInfo){tp*2, p, (center-p)/radius, ray.d};
    }
    virtual RGB getTexture(const CollideInfo& cinfo) const {
        Vector De = Vector( 0 , 0 , 1 );
        Vector Dc = Vector( 0 , 1 , 0 );
        Vector I = Normalize(cinfo.p - center);
        double a = acos( Dot(-I, De ) );
        double b = acos( std::min( std::max( Dot(I, Dc ) / sin( a ) , -1.0 ) , 1.0 ) );
        double u = a / PI , v = b / 2 / PI;
        if ( Dot(I, Cross(Dc, De)) < 0 ) v = 1 - v;
        return material->getSmoothTexture( u , v );
    }
};

// Bezier
class BezierObject: public Object
{
private:
    Vector offset; // offset 对应 bezier的(0, 0, 0)
    Bezier::ptr bezier;
public:
    BezierObject(const Vector& offset, Bezier::ptr bezier): offset(offset), bezier(bezier) {}
    virtual CollideInfo collide(const Ray& ray) const {
        CollideInfo cinfo = bezier->collide(Ray(ray.s-offset, ray.d), false);
        if (cinfo.t > 0) {
            cinfo.p = cinfo.p+offset;
        }
        return cinfo;
    }
    virtual CollideInfo innerCollide(const Ray& ray) const {
        CollideInfo cinfo = bezier->collide(Ray(ray.s-offset, ray.d), true);
        if (cinfo.t > 0) {
            cinfo.p = cinfo.p+offset;
        }
        return cinfo;
    }
    virtual RGB getTexture(const CollideInfo& cinfo) const {
        Vector De = Vector( 0 , 0 , 1 );
        Vector Dc = Vector( 0 , 1 , 0 );
        Vector I = Normalize(cinfo.p - offset - bezier->center());
        double a = acos( Dot(-I, De ) );
        double b = acos( std::min( std::max( Dot(I, Dc ) / sin( a ) , -1.0 ) , 1.0 ) );
        double u = a / PI , v = b / 2 / PI;
        if ( Dot(I, Cross(Dc, De)) < 0 ) v = 1 - v;
        return material->getSmoothTexture( u , v );
    }
};

// 文件对象
class FileObject: public Object
{
private:
    KDTree::ptr kdtree = KDTree::ptr(new KDTree());
    Vector offset;
public:
    FileObject(const Vector& offset, const string& filename): offset(offset) {
        kdtree->loadFromObjFile(filename);
        kdtree->buildTree();
    }
    virtual CollideInfo collide(const Ray& ray) const {
        CollideInfo cinfo = kdtree->collide(Ray(ray.s-offset, ray.d));
        if (cinfo.t > 0) cinfo.p = cinfo.p+offset;
        return cinfo;
    }
    virtual CollideInfo innerCollide(const Ray& ray) const { // FIXME
        CollideInfo cinfo = kdtree->collide(Ray(ray.s-offset, ray.d));
        if (cinfo.t > 0) cinfo.p = cinfo.p+offset;
        return cinfo;
    }
    virtual RGB getTexture(const CollideInfo& cinfo) const {
        return RGB::white(); // FIXME
    }
};

#endif // OBJECT_H