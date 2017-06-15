#ifndef SCENE_H
#define SCENE_H

#include "const.h"
#include "geometry.h"
#include "light.h"
#include "object.h"
#include "camera.h"
#include <opencv2/opencv.hpp>

// 场景
class Scene
{
public:
    Scene(int maxdeep);
    ~Scene();

    Object::ptr putObject(Object::ptr obj);
    Light::ptr putLight(Light::ptr light);
    RGB tracing(const Ray& ray, int remaindeep) const; // 光线追踪
    cv::Mat render(const Camera& camera, int imgsize) const;

private:
    const int maxdeep;
    vector<Object::ptr> objs;
    vector<Light::ptr> lights;
};

#endif // SCENE_H