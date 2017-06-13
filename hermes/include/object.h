#ifndef OBJECT_H
#define OBJECT_H

#include "const.h"
#include "light.h"
#include "geometry.h"

class Scene;

// 物品基类
class Object
{
public:
    Object();
    virtual ~Object();
    typedef shared_ptr<Object> ptr;

    // 渲染，返回RGB
    virtual RGB render(const Ray& ray, double t, const Scene* scene, int deep, const vector<Light::ptr>& possible_ligts) const = 0;

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual double impact(const Ray& ray) const = 0;
};

#endif // OBJECT_H