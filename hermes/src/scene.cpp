#include "scene.h"
#include "time_log.h"

Scene::Scene() {}

Scene::~Scene() {}

Object::ptr Scene::putObject(Object::ptr obj)
{
    objs.push_back(obj);
    return obj;
}

Light::ptr Scene::putLight(Light::ptr light)
{
    lights.push_back(light);
    return light;
}

RGB Scene::tracing(const Ray& ray, int deep) const // 光线追踪
{
    double mint = 1e100;
    Object::ptr minobj;
    for(int i = 0; i < (int)objs.size(); i ++)
    {
        double t = objs[i]->intersect(ray);
        if (t > 0 && mint > t)
        {
            mint = t;
            minobj = objs[i];
        }
    }
    if (minobj)
    {
        return minobj->render(ray, mint, this, deep+1, lights);
    } else {
        return RGB::black();
    }
}

cv::Mat Scene::render(const Camera& camera, int imgsize) const
{
    cv::Mat img(imgsize, imgsize, CV_8UC3);
    setTimePoint("RENDER");

    for(int i = 0; i < imgsize; i ++)
    {
        for(int j = 0; j < imgsize; j ++)
        {
            RGB c = tracing(camera.generateRay((j+0.5)/imgsize, (i+0.5)/imgsize), 0);
            c.min();
            img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*c.b, 255*c.g, 255*c.r);
        }

        imshow("Image", img);
        cv::waitKey(1);
    }

    logTimePoint("RENDER");
    cv::waitKey(0);
    return img;
}
