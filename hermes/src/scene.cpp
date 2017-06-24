#include "scene.hpp"
#include "time_log.hpp"
#include <assert.h>
#include <thread>
#include <mutex>

using namespace std;

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
    if (dcmp(weight.r)<=0 && dcmp(weight.g)<=0 && dcmp(weight.b)<=0)
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
                    if (dcmp(coll.t) > 0 && dcmp(coll.t*coll.t-Length2(linfo.light.s-mincinfo.p)) < 0)
                    {
                        // cout << objIndex(minobj) << " " << objIndex(obj) << endl;
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

    // vector<int> rangeS, rangeT;
    // thread ts[THREADS_COUNT];
    // mutex mtx;
    // splitRange(0, H, THREADS_COUNT, rangeS, rangeT);

    // for(int i = 0; i < THREADS_COUNT; i ++)
    // {
    //     ts[i] = thread([&mtx, this, &img, H, W](int S, int T) {
    //         for(int x = S; x < T; x ++)
    //         {
    //             for(int y = 0; y < W; y ++)
    //             {
    //                 RGB c = RGB::zero();
    //                 vector<Ray> rays = camera->generateRay(x, y);
    //                 for(auto& ray: rays)
    //                     c = c + rayTracing(ray, RGB::one()/rays.size(), NULL, maxdeep);
    //                 c.min();
    //                 // mtx.lock();
    //                 // img.at<cv::Vec3b>(x, y) = cv::Vec3b(255*c.b, 255*c.g, 255*c.r);
    //                 // mtx.unlock();
    //             }
    //             cout << "processing : " << x << endl;
    //         }
    //     }, rangeS[i], rangeT[i]);
    // }
    // for(int i = 0; i < THREADS_COUNT; i ++)
    // {
    //     ts[i].join();
    // }

    logTimePoint("solveRayTracing");
    imshow("Image", img);
    cv::waitKey(0);
    return img;
}

RGB Scene::PPMTracing(int rc, const Ray& ray, RGB weight, Object::ptr inner_obj, int remaindeep, unsigned long long* hash)
{
    if (remaindeep <= 0) return background;
    if (dcmp(weight.r)<=0 && dcmp(weight.g)<=0 && dcmp(weight.b)<=0)
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
            }
        }
        for(auto light: lights)
        {
            double tmp = light->collide(ray);
            if (tmp > 0)
            {
                minlight = light;
            }
        }
    }

    if (!minobj && !minlight) return background;

    RGB ret_color = background;

    if (minlight) {
        ret_color = ret_color + weight.modulate(minlight->color);
        if (hash) *hash = *hash * HASH_MUL + minlight->sample;
    }

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
                    if (dcmp(coll.t) > 0 && dcmp(coll.t*coll.t-Length2(linfo.light.s-mincinfo.p)) < 0)
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

        // 放入hitmap
        if (hash) *hash = *hash * HASH_MUL + minobj->sample;
        else if (inner_obj) {
            double dist = mincinfo.t;
            hitmap->insertPoint(mincinfo, rc, minobj->material->absorb_color.absorb(dist).modulate(weight), minobj->material);
        } else if (minobj->material->hasTexture()) {
            hitmap->insertPoint(mincinfo, rc, weight.modulate(minobj->getTexture(mincinfo)).modulate(minobj->material->plaincolor(mincinfo)), minobj->material);
        } else hitmap->insertPoint(mincinfo, rc, weight.modulate(minobj->material->plaincolor(mincinfo)), minobj->material);

        double reflect_factor = minobj->material->reflect_factor; // 当没有折射的时候发生全反射

        // 折射
        RGB refract_color = RGB::zero();
        double refract_n = inner_obj ? 1.0/minobj->material->refract_n : minobj->material->refract_n;
        Vector refract_v;
        if (Refract(ray.d, mincinfo.n, refract_n, refract_v)) {
            refract_color = PPMTracing(rc, Ray(mincinfo.p, refract_v), weight*minobj->material->refract_factor, inner_obj ? NULL : minobj, remaindeep-1, hash);
        } else reflect_factor += minobj->material->refract_factor;

        // 反射
        Vector r = Reflect(ray.d, mincinfo.n);
        RGB reflect_color = PPMTracing(rc, Ray(mincinfo.p, r), weight*reflect_factor, inner_obj, remaindeep-1, hash);

        if (inner_obj) {
            double dist = mincinfo.t;
            ret_color = ret_color +  minobj->material->absorb_color.absorb(dist).modulate(direct_color + reflect_color + refract_color); // 啤酒定律
        } else {
            ret_color = ret_color + direct_color + reflect_color + refract_color;
        }
    }

    return ret_color;
}

void Scene::PhotonTracing(Photon pho, Object::ptr inner_obj, int remaindeep)
{
    if (remaindeep <= 0) return;
    if (dcmp(pho.power.r)<=0 && dcmp(pho.power.g)<=0 && dcmp(pho.power.b)<=0) return;
    assert(dcmp(Length2(pho.movement.d)-1) == 0);

    double mint = INF;
    CollideInfo mincinfo = NoCollide;
    Object::ptr minobj;

    if (inner_obj) // 内部视线
    {
        minobj = inner_obj;
        mincinfo = inner_obj->innerCollide(pho.movement);
        if (dcmp(mincinfo.t)<=0) return;
    } else {
        for(auto obj: objs)
        {
            CollideInfo tmp = obj->collide(pho.movement);
            if (tmp.t > 0 && mint > tmp.t)
            {
                mint = tmp.t;
                mincinfo = tmp;
                minobj = obj;
            }
        }
    }

    if (!minobj) return;

    if (inner_obj) {
        double dist = mincinfo.t;
        pho.power = minobj->material->absorb_color.absorb(dist).modulate(pho.power); // 啤酒定律
    }

    hitmap->insertPhoton(pho, mincinfo, R);

    // 漫反射
    RGB diffuse_power = minobj->material->plaincolor(mincinfo).modulate(pho.power)*minobj->material->diffuse_factor;
    if (minobj->material->hasTexture()) diffuse_power = diffuse_power.modulate(minobj->getTexture(mincinfo));
    PhotonTracing((Photon){diffuse_power, Ray(mincinfo.p, GetRandomVector(mincinfo.n))}, inner_obj, remaindeep-1);

    double reflect_factor = minobj->material->reflect_factor; // 当没有折射的时候发生全反射

    // 折射
    double refract_n = inner_obj ? 1.0/minobj->material->refract_n : minobj->material->refract_n;
    Vector refract_v;
    if (Refract(pho.movement.d, mincinfo.n, refract_n, refract_v)) {
        assert(dcmp(Length2(refract_v)-1) == 0);
        PhotonTracing((Photon){pho.power*minobj->material->refract_factor, Ray(mincinfo.p, refract_v)}, inner_obj ? NULL : minobj, remaindeep-1);
    } else reflect_factor += minobj->material->refract_factor;

    // 反射
    RGB reflect_power = minobj->material->plaincolor(mincinfo).modulate(pho.power)*reflect_factor;
    if (minobj->material->hasTexture()) reflect_power = reflect_power.modulate(minobj->getTexture(mincinfo));
    Vector r = Reflect(pho.movement.d, mincinfo.n);
    assert(dcmp(Length2(r)-1) == 0);
    PhotonTracing((Photon){reflect_power, Ray(mincinfo.p, r)}, inner_obj, remaindeep-1);
}

cv::Mat Scene::PPMRender()
{
    const int H = camera->getHeight();
    const int W = camera->getWidth();

    cv::Mat img(H, W, CV_8UC3);
    setTimePoint("PPMRender");

    unsigned long long *hash = new unsigned long long[H*W];
    if (camera->needSmooth())
    {
        for(int i = 0; i < H; i ++)
        {
            cout << "hashing : " << i << " / " << H << endl;
            for(int j = 0; j < W; j ++)
            {
                vector<Ray> rays = camera->generateRay(i, j);
                hash[i*W+j] = 0;
                for(auto& ray: rays)
                    PPMTracing(i*W+j, ray, RGB::one()/rays.size(), NULL, maxdeep, &hash[i*W+j]);
            }
        }
    }
    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    RGB *origin_rgbs = new RGB[H*W];
    for(int i = 0; i < H; i ++)
    {
        cout << "sampling : " << i << " / " << H << endl;
        for(int j = 0; j < W; j ++)
        {
            bool smooth = false;
            if (camera->needSmooth())
            {
                smooth |= i > 0 && hash[i*W+j] != hash[(i-1)*W+j];
                smooth |= j > 0 && hash[i*W+j] != hash[i*W+j-1];
                smooth |= i < H-1 && hash[i*W+j] != hash[(i+1)*W+j];
                smooth |= j < W-1 && hash[i*W+j] != hash[i*W+j+1];
            }

            vector<Ray> rays;
            if (smooth) rays = camera->generateSmoothRay(i, j);
            else rays = camera->generateRay(i, j);

            RGB c = RGB::zero();
            for(auto& ray: rays)
                c = c + PPMTracing(i*W+j, ray, RGB::one()/rays.size(), NULL, maxdeep, NULL);
            origin_rgbs[i*W+j] = c;
            c.min();
            img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*c.b, 255*c.g, 255*c.r);
        }

        cv::imshow("Image", img);
        cv::waitKey(1);
    }
    delete[] hash;
    cv::imwrite("noPPM.png", img);
    cv::destroyAllWindows();

    logTimePoint("PPMRender");

    setTimePoint("buildHitmap");
    cout << hitmap->getStoredPoints() << endl;
    hitmap->buildTree();
    logTimePoint("buildHitmap");

    setTimePoint("Photon");
    R = origin_R;
    for(int iter = 0, total_times = 0; iter < 30; iter ++)
    {
        cout << "iter : " << iter << " R : " << R << endl;
        for(int times = 0; times <= 50*10000; times ++, total_times ++)
        {
            for(auto light: lights)
            {
                Photon pho = light->genPhoton(times);
                PhotonTracing(pho, NULL, maxdeep);
            }
            if (times % 2000 == 0) cout << "Photon " << times << " " << iter << endl;
        }

        RGB *ppm_rgbs = new RGB[H*W];
        HitPoint* points = hitmap->getHitPoints();
        int stored_points = hitmap->getStoredPoints();
        for(int i = 0; i < stored_points; i ++)
        {
            int rc = points[i].rc;
            ppm_rgbs[rc] = ppm_rgbs[rc] + points[i].color.modulate(points[i].weight);
        }
        cv::Mat photon_img(H, W, CV_8UC3, cv::Scalar(0, 0, 0));
        for(int i = 0; i < H; i ++)
            for(int j = 0; j < W; j ++)
            {
                int rc = i*W+j;
                ppm_rgbs[rc] = ppm_rgbs[rc]/total_times/R/R;
                RGB rgb = ppm_rgbs[rc];
                rgb.min();
                photon_img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*rgb.b, 255*rgb.g, 255*rgb.r);
            }
        char buf[100];
        sprintf(buf, "%dphoton.png", iter);
        cv::imwrite(buf, photon_img);

        cv::Mat merge_img(H, W, CV_8UC3, cv::Scalar(0, 0, 0));
        for(int i = 0; i < H; i ++)
            for(int j = 0; j < W; j ++)
            {
                RGB rgb = ppm_rgbs[i*W+j] + origin_rgbs[i*W+j];
                rgb.min();
                merge_img.at<cv::Vec3b>(i, j) = cv::Vec3b(255*rgb.b, 255*rgb.g, 255*rgb.r);
            }
        sprintf(buf, "%dmerge.png", iter);
        cv::imwrite(buf, merge_img);

        delete[] ppm_rgbs;

        // R = R*sqrt((iter+alpha)/(iter+1));
    }
    logTimePoint("Photon");

    delete[] origin_rgbs;

    return img;
}