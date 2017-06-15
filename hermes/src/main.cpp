#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <assert.h>

#include "time_log.h"
#include "scene.h"
#include "sphere_object.h"
#include "line_light.h"
#include "phong_material.h"
#include "plane_object.h"
#include "grid_material.h"

using namespace std;

void test();

int main()
{
    setTimePoint("PROCESS");

    cout << "Hello World" << endl;

    // test();
    // return 0;

    Scene scene;
    scene.putLight(Light::ptr(new LineLight(RGB::white(), Vector(-1, 1, -1))));
    scene.putObject(Object::ptr(new SphereObject(Point(-1, 0, 0), 0.7, Material::ptr(new PhongMaterial(RGB::red(), RGB::white(), 15)))));
    scene.putObject(Object::ptr(new SphereObject(Point(1, 0, 0), 0.7, Material::ptr(new PhongMaterial(RGB::red(), RGB::white(), 15)))));
    scene.putObject(Object::ptr(new PlaneObject(Point(0, 0, 1), 1.5, Material::ptr(new GridMaterial(3)))));
    
    Camera camera(Point(0, -5, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60);
    cv::Mat img = scene.render(camera, 500);
    cv::imwrite("temp.png", img);

    logTimePoint("PROCESS");
    return 0;
}

void test()
{
    assert(dcmp(Dot(Vector(0, 0, 1), Vector(0, 1, 0))) == 0);
    assert(dcmp(Dot(Vector(0, 0, 1), Vector(0, 0, 1))) == 1);
    assert(dcmp(Dot(Vector(0, 1, 1), Vector(0, 0, 1))) == 1);

    double t = IntersectWithSurface(Ray(Point(0, -5, 0), Vector(0, 1, 0)), Vector(0, -1, 0), 0);
    assert(dcmp(t) != 0);
    cout << t << endl;
}