#include "scene.h"

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
    cout << ray.d.x << " " << ray.d.y << " " << ray.d.z << endl;
    return RGB(1, 0, 0);
}

cv::Mat Scene::render(const Point& view_point, const Rect& view_rect, int output_height, int output_width) const
{
    cv::Mat img(output_height, output_width, CV_8UC3);
    for(int i = 0; i < output_height; i ++)
        for(int j = 0; j < output_width; j ++)
        {
            Point p = view_rect.ltpoint + (view_rect.rtpoint-view_rect.ltpoint)*(j+0.5)/output_width + (view_rect.lbpoint-view_rect.ltpoint)*(i+0.5)/output_height;
            RGB c = tracing(Ray(view_point, p-view_point), 0);
            img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*c.b, 255*c.g, 255*c.r);
        }
    return img;
}
