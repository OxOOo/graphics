// 常量定义

#ifndef CONST_H
#define CONST_H

#include <vector>
#include <memory>

using namespace std;

struct RGB
{
    double r, g, b; // [0, 1]
    RGB() {}
    RGB(double r, double g, double b): r(r), g(g), b(g) {}
};

#endif // CONST_H