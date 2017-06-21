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

    Scene scene(6, 4);
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0.15;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1.1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 1.1;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.3;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    right_m->reflect_factor = 0.1;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 1.1;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.3;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    top_m->reflect_factor = 0.15;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 1.1;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 2, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 1.1;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.3;
    front_m->absorb_color = RGB::white();

    scene.putLight(Light::ptr(new RectLight(Point(0, 0, 1), Vector(-0.2, 0, 0), Vector(0, 0.2, 0))))->color = RGB::white();

    auto sphere1_m = scene.putObject(Object::ptr(new SphereObject(Point(-0.7, 1, 0), 0.3)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere1_m->reflect_factor = 1;
    sphere1_m->refract_factor = 0;
    sphere1_m->diffuse_factor = 0;
    sphere1_m->refract_n = 1.5;
    sphere1_m->high_light = 0.4;
    sphere1_m->absorb_color = RGB(0, 0, 0);

    auto sphere2_m =scene.putObject(Object::ptr(new SphereObject(Point(0.65, 0, -0.5), 0.2)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere2_m->reflect_factor = 0.1;
    sphere2_m->refract_factor = 1;
    sphere2_m->diffuse_factor = 0;
    sphere2_m->refract_n = 1.5;
    sphere2_m->high_light = 0.6;
    sphere2_m->absorb_color = RGB(0, 0, 0);

    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 500)));
    
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