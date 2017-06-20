#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <assert.h>

#include "time_log.hpp"
#include "scene.hpp"
#include "object.hpp"
#include "light.hpp"
#include "material.hpp"

using namespace std;

void test();

int main()
{
    setTimePoint("PROCESS");

    cout << "Hello World" << endl;

    // test();
    // return 0;

    Scene scene(6);
    scene.putLight(Light::ptr(new ConeLight(Point(10, 10, 10), Vector(-1, -1, -1), 70)))->color = RGB::white();

    auto sphere1_m = scene.putObject(Object::ptr(new SphereObject(Point(-1, 0, 0), 0.7)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere1_m->reflect_factor = 0.4;
    sphere1_m->refract_factor = 0.6;
    sphere1_m->diffuse_factor = 0.3;
    sphere1_m->refract_n = 1.7;
    sphere1_m->high_light = 0.4;
    sphere1_m->absorb_color = RGB(0, 0, 0);

    auto sphere2_m =scene.putObject(Object::ptr(new SphereObject(Point(1, 0, 0), 0.7)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere2_m->reflect_factor = 0.25;
    sphere2_m->refract_factor = 0.6;
    sphere2_m->diffuse_factor = 0.5;
    sphere2_m->refract_n = 1.5;
    sphere2_m->high_light = 0.6;
    sphere2_m->absorb_color = RGB(1, 0, 0);

    auto object_m =scene.putObject(Object::ptr(new PlaneObject(Point(0, 0, 1), 1)))->material = Material::ptr(new GridMaterial(3));
    object_m->reflect_factor = 0.15;
    object_m->refract_factor = 0;
    object_m->diffuse_factor = 1.1;
    object_m->refract_n = 1.5;
    object_m->high_light = 0.3;
    object_m->absorb_color = RGB::white();

    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -5, 2), Vector(0, 1, -0.2), Vector(0, 0, 1), 60, 500)));
    
    cv::Mat img = scene.renderRayTracing();
    cv::imwrite("temp.png", img);

    logTimePoint("PROCESS");
    return 0;
}

void test()
{
    assert(dcmp(Dot(Vector(0, 0, 1), Vector(0, 1, 0))) == 0);
    assert(dcmp(Dot(Vector(0, 0, 1), Vector(0, 0, 1))) == 1);
    assert(dcmp(Dot(Vector(0, 1, 1), Vector(0, 0, 1))) == 1);

    double t = CollideWithSurface(Ray(Point(0, -5, 0), Vector(0, 1, 0)), Vector(0, -1, 0), 0);
    assert(dcmp(t) != 0);
    cout << t << endl;
}