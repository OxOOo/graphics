#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <assert.h>
#include <ctime>

#include "time_log.hpp"
#include "scene.hpp"
#include "object.hpp"
#include "light.hpp"
#include "material.hpp"
#include "bezier.hpp"

using namespace std;

void scene1(); // 珠子

void scene2(); // 兔子

void scene3(); // Bezier

void scene4();

void scene5();

void scene6(); // 景深摄像机

int main()
{
    setTimePoint("PROCESS");

    cout << "Hello World" << endl;
    // srand(time(0));

    // Bezier B;
    // B.append(Point(0, 0, 0));
    // B.append(Point(1, 0, 0));
    // B.append(Point(0, 0, 0.5));
    // B.append(Point(0, 0, 1));
    // cv::Mat tmp = B.draw();
    // cv::imwrite("temp.png", tmp);
    // return 0;

    scene6();

    logTimePoint("PROCESS");
    return 0;
}

void scene1()
{
    Scene scene;
    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 600, 3)));
    scene.putLight(Light::ptr(new RectLight(Point(0, 0.6, 1), Vector(-0.2, 0, 0), Vector(0, 0.2, 0))))->color = RGB::white();
    
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 1;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.7;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9ACD32")));
    right_m->reflect_factor = 0.1;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 1;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.7;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    top_m->reflect_factor = 0.5;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 1;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 2, 10)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 0.6;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.3;
    front_m->absorb_color = RGB::white();

    auto sphere1_m = scene.putObject(Object::ptr(new SphereObject(Point(-0.7, 1, 0), 0.3)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere1_m->reflect_factor = 1;
    sphere1_m->refract_factor = 0;
    sphere1_m->diffuse_factor = 0;
    sphere1_m->refract_n = 1.5;
    sphere1_m->high_light = 0.4;
    sphere1_m->absorb_color = RGB(0, 0, 0);

    auto sphere2_m = scene.putObject(Object::ptr(new SphereObject(Point(0.65, 0.4, -0.7), 0.25)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere2_m->reflect_factor = 0.0; // 0.1
    sphere2_m->refract_factor = 1;
    sphere2_m->diffuse_factor = 0;
    sphere2_m->refract_n = 1.5;
    sphere2_m->high_light = 0.6;
    sphere2_m->absorb_color = RGB(0.8, 0.2, 0);

    auto sphere3_m = scene.putObject(Object::ptr(new SphereObject(Point(-0.4, 0, -0.8), 0.2)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    sphere3_m->reflect_factor = 0.0; // 0.3
    sphere3_m->refract_factor = 1;
    sphere3_m->diffuse_factor = 0;
    sphere3_m->refract_n = 1.4;
    sphere3_m->high_light = 0.3;
    sphere3_m->absorb_color = RGB(0, 0, 0);
    
    cv::Mat img = scene.PPMRender(); // scene.renderRayTracing();
    cv::imwrite("temp.png", img);
}

void scene2()
{
    Scene scene;
    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 1080, 3)));
    scene.putLight(Light::ptr(new RectLight(Point(1, 0, 0), Vector(0, 0.2, 0), Vector(0, 0, 0.2))))->color = RGB::white();
    
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 20)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 1;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.7;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9ACD32")));
    right_m->reflect_factor = 0.9;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 0.3;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.7;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    top_m->reflect_factor = 0;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 0.8;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 2, 10)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 0.6;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.3;
    front_m->absorb_color = RGB::white();

    auto file_m = scene.putObject(Object::ptr(new FileObject(Vector(0, 1, -0.5), "bunny.obj")))->material = Material::ptr(new PlainMaterial(RGB("FFD39B")));
    file_m->reflect_factor = 0;
    file_m->refract_factor = 0;
    file_m->diffuse_factor = 1;
    file_m->refract_n = 1.4;
    file_m->high_light = 0.6;
    file_m->absorb_color = RGB(0, 0, 0);

    cv::Mat img = scene.PPMRender(); // scene.renderRayTracing();
    cv::imwrite("temp.png", img);
}

void scene3()
{
    Scene scene;
    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 1080, 3)));
    scene.putLight(Light::ptr(new RectLight(Point(1, 0, 0), Vector(0, 0.2, 0), Vector(0, 0, -0.2))))->color = RGB::white();
    
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 0.6;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.7;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9ACD32")));
    right_m->reflect_factor = 0.5;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 0;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.7;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9B30FF")));
    top_m->reflect_factor = 0.1;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 1;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 2, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 0.8;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.4;
    front_m->absorb_color = RGB::white();

    auto bezier = Bezier::ptr(new Bezier());
    bezier->append(Point(0, 0, 0));
    bezier->append(Point(1, 0, 0));
    bezier->append(Point(0, 0, 1));
    bezier->maintain(0.7);
    auto bezier_m = scene.putObject(Object::ptr(new BezierObject(Vector(0, 0.5, -1), bezier)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    bezier_m->reflect_factor = 0;
    bezier_m->refract_factor = 1;
    bezier_m->diffuse_factor = 0;
    bezier_m->refract_n = 1.5;
    bezier_m->high_light = 0.3;
    bezier_m->absorb_color = RGB(-1, -1, -1);

    cv::Mat img = scene.PPMRender(); // scene.renderRayTracing();
    cv::imwrite("temp.png", img);
}

void scene4()
{
    Scene scene;
    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 1080, 3)));
    scene.putLight(Light::ptr(new RectLight(Point(1, 0, 0.5), Vector(0, 0.2, 0), Vector(0, 0, -0.2))))->color = RGB::white();
    
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 0.1)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 0.6;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.7;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9ACD32")));
    right_m->reflect_factor = 0.5;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 0;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.7;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9B30FF")));
    top_m->reflect_factor = 0.1;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 1;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 2, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 0.8;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.4;
    front_m->absorb_color = RGB::white();

    auto sphere1_m = scene.putObject(Object::ptr(new SphereObject(Point(-0.3, 1, -0.5), 0.5)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    sphere1_m->reflect_factor = 0;
    sphere1_m->refract_factor = 0;
    sphere1_m->diffuse_factor = 1;
    sphere1_m->refract_n = 1.5;
    sphere1_m->high_light = 0.4;
    sphere1_m->absorb_color = RGB(0, 0, 0);
    sphere1_m->texture = cv::imread("marble.bmp");

    auto bezier1 = Bezier::ptr(new Bezier());
    bezier1->append(Point(0, 0, 0));
    bezier1->append(Point(1, 0, 0));
    bezier1->append(Point(0, 0, 0.5));
    bezier1->append(Point(0, 0, 1));
    bezier1->maintain(0.7);
    auto bezier1_m = scene.putObject(Object::ptr(new BezierObject(Vector(0.5, -0.3, -1), bezier1)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    bezier1_m->reflect_factor = 1;
    bezier1_m->refract_factor = 0;
    bezier1_m->diffuse_factor = 0;
    bezier1_m->refract_n = 1.5;
    bezier1_m->high_light = 0.3;
    bezier1_m->absorb_color = RGB(0, 0, 0);

    auto bezier2 = Bezier::ptr(new Bezier());
    bezier2->append(Point(0, 0, 0));
    bezier2->append(Point(1, 0, 0));
    bezier2->append(Point(0, 0, 1));
    bezier2->maintain(0.7);
    auto bezier2_m = scene.putObject(Object::ptr(new BezierObject(Vector(-0.5, -0.5, -1), bezier2)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    bezier2_m->reflect_factor = 0;
    bezier2_m->refract_factor = 0;
    bezier2_m->diffuse_factor = 1;
    bezier2_m->refract_n = 1.5;
    bezier2_m->high_light = 0.3;
    bezier2_m->absorb_color = RGB(0, 0, 0);
    bezier2_m->texture = cv::imread("marble.bmp");

    cv::Mat img = scene.PPMRender(); // scene.renderRayTracing();
    cv::imwrite("temp.png", img);
}

void scene5()
{
    Scene scene;
    scene.setCamera(Camera::ptr(new PerspectiveCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 600, 3)));
    scene.putLight(Light::ptr(new RectLight(Point(1, 0, 0), Vector(0, 0.2, 0), Vector(0, 0, 0.2))))->color = RGB::white();
    
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 20)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 1;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.7;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9ACD32")));
    right_m->reflect_factor = 0.9;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 0.3;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.7;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    top_m->reflect_factor = 0;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 0.8;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 2, 10)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 0.6;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.3;
    front_m->absorb_color = RGB::white();

    auto file_m = scene.putObject(Object::ptr(new FileObject(Vector(0, 1, -0.5), "bunny.obj")))->material = Material::ptr(new PlainMaterial(RGB::white()));
    file_m->reflect_factor = 0;
    file_m->refract_factor = 1;
    file_m->diffuse_factor = 0;
    file_m->refract_n = 1.4;
    file_m->high_light = 0.6;
    file_m->absorb_color = RGB(-1, -1, -1);

    cv::Mat img = scene.PPMRender(); // scene.renderRayTracing();
    cv::imwrite("temp.png", img);
}

void scene6()
{
    Scene scene;
    DeepCamera *camera = (DeepCamera*)scene.setCamera(Camera::ptr(new DeepCamera(Point(0, -2, 0), Vector(0, 1, 0), Vector(0, 0, 1), 60, 600))).get();
    camera->lens_R = 0.05;
    camera->lens_dis = 0.1;
    camera->item_dis = 2.2;
    camera->sample_num = 30;
    scene.putLight(Light::ptr(new RectLight(Point(0, 0.6, 1), Vector(-0.2, 0, 0), Vector(0, 0.2, 0))))->color = RGB::white();
    
    auto floor_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, 1), 1, 20)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    floor_m->reflect_factor = 0;
    floor_m->refract_factor = 0;
    floor_m->diffuse_factor = 1;
    floor_m->refract_n = 1.5;
    floor_m->high_light = 0.3;
    floor_m->absorb_color = RGB::white();
    floor_m->texture = cv::imread("floor.bmp");

    auto left_m = scene.putObject(Object::ptr(new PlaneObject(Vector(1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("ADD8E6")));
    left_m->reflect_factor = 0.1;
    left_m->refract_factor = 0;
    left_m->diffuse_factor = 1;
    left_m->refract_n = 1.5;
    left_m->high_light = 0.7;
    left_m->absorb_color = RGB::white();

    auto right_m = scene.putObject(Object::ptr(new PlaneObject(Vector(-1, 0, 0), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB("9ACD32")));
    right_m->reflect_factor = 0.1;
    right_m->refract_factor = 0;
    right_m->diffuse_factor = 1;
    right_m->refract_n = 1.5;
    right_m->high_light = 0.7;
    right_m->absorb_color = RGB::white();

    auto top_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, 0, -1), 1, 10)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    top_m->reflect_factor = 0.8;
    top_m->refract_factor = 0;
    top_m->diffuse_factor = 0;
    top_m->refract_n = 1.5;
    top_m->high_light = 0.3;
    top_m->absorb_color = RGB::white();

    auto front_m = scene.putObject(Object::ptr(new PlaneObject(Vector(0, -1, 0), 3, 10)))->material = Material::ptr(new PlainMaterial(RGB::red()));
    front_m->reflect_factor = 0;
    front_m->refract_factor = 0;
    front_m->diffuse_factor = 0.6;
    front_m->refract_n = 1.5;
    front_m->high_light = 0.3;
    front_m->absorb_color = RGB::white();

    auto file_m = scene.putObject(Object::ptr(new FileObject(Vector(0, 2.5, -0.5), "bunny.obj")))->material = Material::ptr(new PlainMaterial(RGB("FFD39B")));
    file_m->reflect_factor = 0;
    file_m->refract_factor = 0;
    file_m->diffuse_factor = 1;
    file_m->refract_n = 1.4;
    file_m->high_light = 0.6;
    file_m->absorb_color = RGB(0, 0, 0);

    auto sphere1_m = scene.putObject(Object::ptr(new SphereObject(Point(-0.2, 0.2, -0.7), 0.3)))->material = Material::ptr(new PlainMaterial(RGB::white()));
    sphere1_m->reflect_factor = 0;
    sphere1_m->refract_factor = 1;
    sphere1_m->diffuse_factor = 0;
    sphere1_m->refract_n = 1.5;
    sphere1_m->high_light = 0.5;
    sphere1_m->absorb_color = RGB(0, 0, 0);
    sphere1_m->texture = cv::imread("marble.bmp");

    cv::Mat img = scene.PPMRender(); // scene.renderRayTracing();
    cv::imwrite("temp.png", img);
}
