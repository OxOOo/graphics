// 常量定义
#pragma once

#include <vector>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
#include <assert.h>
#include "geometry.hpp"
#include "rgb.hpp"

using namespace std;

const double WEIGHT_EPS = 1e-2;
const int THREADS_COUNT = 2;

struct CollideInfo
{
    double t;
    Point p;
    Vector n;
    Vector rayd; // 碰撞入射方向
};

static CollideInfo NoCollide = (CollideInfo){-1, Point(), Vector(), Vector()};

struct LightInfo
{
    RGB rgb;
    Ray light;
};

double RAND();

void splitRange(int S, int T, int n, vector<int>& rangeS, vector<int>& rangeT);