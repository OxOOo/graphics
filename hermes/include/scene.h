#ifndef SCENE_H
#define SCENE_H

#include "const.h"
#include "geometry.h"
#include "light.h"
#include "object.h"
#include <opencv2/opencv.hpp>

// 场景
class Scene
{
public:
    Scene();
    ~Scene();

    Object::ptr putObject(Object::ptr obj);
    Light::ptr putLight(Light::ptr light);
    RGB tracing(const Ray& ray, int deep) const; // 光线追踪
    cv::Mat render(const Point& view_point, const Rect& view_rect, int output_height, int output_width) const;

private:
    vector<Object::ptr> objs;
    vector<Light::ptr> lights;
};

#endif // SCENE_H