#ifndef OBJECT_H
#define OBJECT_H

#include "const.h"
#include "light.h"
#include "geometry.h"
#include "material.h"

class Scene;

// 物品基类
class Object
{
public:
    const Material::ptr material;
    Object(Material::ptr material): material(material) {}
    virtual ~Object() {}
    typedef shared_ptr<Object> ptr;
    
    struct IntersectInfo
    {
        double t;
        Point p;
        Vector n;
    };

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual IntersectInfo intersect(const Ray& ray) const = 0;
};

static Object::IntersectInfo NoIntersect = (Object::IntersectInfo){-1, Point(), Vector()};

#endif // OBJECT_H