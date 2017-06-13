#ifndef SCENE_H
#define SCENE_H

#include "const.h"
#include "geometry.h"

// 场景
class Scene
{
public:
    Scene();
    ~Scene();

    RGB tracing(const Ray& ray, int deep) const; // 光线追踪
};

#endif // SCENE_H