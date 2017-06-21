#include "scene.hpp"
#include "time_log.hpp"
#include <assert.h>
#include <thread>
#include <mutex>

using namespace std;

Scene::Scene(int maxdeep, int shade_quality): maxdeep(maxdeep), shade_quality(shade_quality) {}

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

Camera::ptr Scene::setCamera(Camera::ptr camera)
{
    this->camera = camera;
    return camera;
}

int Scene::objIndex(Object::ptr obj) const
{
    for(int i = 0; i < (int)objs.size(); i ++)
        if (obj.get() == objs[i].get())
            return i;
    return -1;
}

RGB Scene::rayTracing(const Ray& ray, const RGB& weight, Object::ptr inner_obj, int remaindeep) const // 光线追踪
{
    if (remaindeep <= 0) return background;
    if (dcmp(weight.r-WEIGHT_EPS)<=0 && dcmp(weight.g-WEIGHT_EPS)<=0 && dcmp(weight.b-WEIGHT_EPS)<=0)
        return background;

    double mint = INF;
    CollideInfo mincinfo = NoCollide;
    Object::ptr minobj;
    Light::ptr minlight;

    if (inner_obj) // 内部视线
    {
        minobj = inner_obj;
        mincinfo = inner_obj->innerCollide(ray);
    } else {
        for(auto obj: objs)
        {
            CollideInfo tmp = obj->collide(ray);
            if (tmp.t > 0 && mint > tmp.t)
            {
                mint = tmp.t;
                mincinfo = tmp;
                minobj = obj;
                minlight.reset();
            }
        }
        for(auto light: lights)
        {
            double tmp = light->collide(ray);
            if (tmp > 0)
            {
                mint = tmp;
                minobj.reset();
                minlight = light;
            }
        }
    }

    if (!minobj && !minlight) return background;

    if (minobj)
    {
        // 直接光
        RGB direct_color = RGB::zero();
        for(auto light: lights)
        {
            vector<LightInfo> ls = light->targetLights(mincinfo.p, shade_quality);
            for(auto linfo: ls)
            {
                bool reachable = true;
                for(auto obj: objs)
                {
                    CollideInfo coll = obj->collide(Ray(mincinfo.p, -linfo.light.d));
                    if (dcmp(coll.t) > 0 && dcmp(coll.t-Length(linfo.light.s-mincinfo.p)) < 0)
                    {
                        reachable = false;
                        break;
                    }
                }
                if (reachable) {
                    direct_color = direct_color + minobj->material->sample(linfo, mincinfo)/ls.size();
                }
            }
        }
        direct_color = weight.modulate(direct_color);
        if (minobj->material->hasTexture()) {
            direct_color = direct_color.modulate(minobj->getTexture(mincinfo));
        }

        double reflect_factor = minobj->material->reflect_factor; // 当没有折射的时候发生全反射

        // 折射
        RGB refract_color = RGB::zero();
        double refract_n = inner_obj ? 1.0/minobj->material->refract_n : minobj->material->refract_n;
        Vector refract_v;
        if (Refract(ray.d, mincinfo.n, refract_n, refract_v)) {
            refract_color = rayTracing(Ray(mincinfo.p, refract_v), weight*minobj->material->refract_factor, inner_obj ? NULL : minobj, remaindeep-1);
        } else reflect_factor += minobj->material->refract_factor;

        // 反射
        Vector r = Reflect(ray.d, mincinfo.n);
        RGB reflect_color = rayTracing(Ray(mincinfo.p, r), weight*reflect_factor, inner_obj, remaindeep-1);

        if (inner_obj) {
            double dist = mincinfo.t;
            return  minobj->material->absorb_color.absorb(dist).modulate(direct_color + reflect_color + refract_color); // 啤酒定律
        } else {
            return direct_color + reflect_color + refract_color;
        }
    } else {
        return weight.modulate(minlight->color);
    }

    return background;
}

cv::Mat Scene::renderRayTracing()
{
    const int H = camera->getHeight();
    const int W = camera->getWidth();

    cv::Mat img(H, W, CV_8UC3);
    setTimePoint("solveRayTracing");

    for(int i = 0; i < H; i ++)
    {
        for(int j = 0; j < W; j ++)
        {
            RGB c = RGB::zero();
            vector<Ray> rays = camera->generateRay(i, j);
            for(auto& ray: rays)
                c = c + rayTracing(ray, RGB::one()/rays.size(), NULL, maxdeep);
            c.min();
            img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*c.b, 255*c.g, 255*c.r);
        }

        imshow("Image", img);
        cv::waitKey(1);
    }

    // int x = 0, y = 0;
    // thread ts[THREADS_COUNT];
    // mutex mtx;
    // for(int i = 0; i < THREADS_COUNT; i ++)
    // {
    //     ts[i] = thread([&mtx, this, &x, &y, &img, H, W]() {
    //         while(true)
    //         {
    //             mtx.lock();
    //             y ++;
    //             x += y / W;
    //             y %= W;
    //             if (y == 0) cout << x << endl;
    //             const int i = x, j = y;
    //             mtx.unlock();

    //             if (x >= H) return;

    //             RGB c = RGB::zero();
    //             vector<Ray> rays = camera->generateRay(i, j);
    //             for(auto& ray: rays)
    //                 c = c + rayTracing(ray, RGB::one()/rays.size(), NULL, maxdeep);
    //             c.min();
    //             img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*c.b, 255*c.g, 255*c.r);
    //         }
    //     });
    // }
    // for(int i = 0; i < THREADS_COUNT; i ++)
    // {
    //     ts[i].join();
    // }

    logTimePoint("solveRayTracing");
    cv::waitKey(0);
    return img;
}