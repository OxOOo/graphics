#ifndef SCENE_H
#define SCENE_H

#include "const.hpp"
#include "geometry.hpp"
#include "light.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "hitmap.hpp"
#include <opencv2/opencv.hpp>

// 场景
class Scene
{
public:
    Scene();
    ~Scene();

    const RGB background = RGB(0.0, 0.0, 0.0);

    Object::ptr putObject(Object::ptr obj);
    Light::ptr putLight(Light::ptr light);
    Camera::ptr setCamera(Camera::ptr camera);

    RGB rayTracing(const Ray& ray, const RGB& weight, Object::ptr inner_obj, int remaindeep) const; // 光线追踪
    cv::Mat renderRayTracing();

    RGB PPMTracing(int rc, const Ray& ray, const RGB& weight, Object::ptr inner_obj, int remaindeep);
    void PhotonTracing(Photon pho, Object::ptr inner_obj, int remaindeep);
    cv::Mat PPMRender();

private:
    int objIndex(Object::ptr obj) const;

    const int maxdeep = 6;
    const int shade_quality = 1;
    const double R = 0.02;
    HitMap::ptr hitmap = HitMap::ptr(new HitMap(200*10000));

    vector<Object::ptr> objs;
    vector<Light::ptr> lights;
    Camera::ptr camera;
};

#endif // SCENE_H