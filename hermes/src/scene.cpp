#include "scene.h"
#include "time_log.h"

Scene::Scene(int maxdeep): maxdeep(maxdeep) {}

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

RGB Scene::tracing(const Ray& ray, int remaindeep) const // 光线追踪
{
    double mint = 1e100;
    Object::IntersectInfo mininfo;
    Object::ptr minobj;
    for(int i = 0; i < (int)objs.size(); i ++)
    {
        Object::IntersectInfo tmp = objs[i]->intersect(ray);
        if (tmp.t > 0 && mint > tmp.t)
        {
            mint = tmp.t;
            mininfo = tmp;
            minobj = objs[i];
        }
    }
    if (minobj)
    {
        vector<Light::ptr> possible_lights;
        for(auto light: lights)
        {
            bool reachable = true;
            for(int i = 0; i < (int)objs.size() && reachable; i ++)
                if (!light->reachable(mininfo.p, objs[i].get()))
                    reachable = false;
            if (reachable) possible_lights.push_back(light);
        }
        RGB rgb;
        if (dcmp(1-minobj->material->reflectiveness) > 0) {
            RGB mrgb = minobj->material->sample(ray, mininfo.p, mininfo.n, possible_lights);
            rgb = rgb + mrgb*(1-minobj->material->reflectiveness);
        }
        if (dcmp(minobj->material->reflectiveness) > 0 && remaindeep > 0) {
            Vector r = mininfo.n*(-2 * Dot(mininfo.n, ray.d)) + ray.d;
            RGB reflected = tracing(Ray(mininfo.p, r), remaindeep-1);
            rgb = rgb + reflected*minobj->material->reflectiveness;
        }
        return rgb;
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
            RGB c;
            for(double x = j+0.25; x < j+1; x += 0.5)
                for(double y = i+0.25; y < i+1; y += 0.5)
                    c = c + tracing(camera.generateRay((j+0.5)/imgsize, (i+0.5)/imgsize), maxdeep);
            c = c*0.25;
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
