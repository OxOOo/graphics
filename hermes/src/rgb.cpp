#include "rgb.hpp"
#include "const.hpp"
#include <assert.h>

int hex(char c)
{
    if ('a' <= c && c <= 'z') c = c - 'a' + 'A';
    if ('0' <= c && c <= '9') return c-'0';
    return c-'A'+10;
}

RGB::RGB(): r(0), g(0), b(0) {}
RGB::RGB(double r, double g, double b): r(r), g(g), b(b) {}
RGB::RGB(cv::Vec3b color)
{
    b = color[0]/255.0;
    g = color[1]/255.0;
    r = color[2]/255.0;
}
RGB::RGB(std::string color)
{
    assert(color.length() == 6);
    r = (hex(color[0])*16+hex(color[1]))/255.0;
    g = (hex(color[2])*16+hex(color[3]))/255.0;
    b = (hex(color[4])*16+hex(color[4]))/255.0;
}

RGB RGB::modulate(const RGB& c) const
{
    return RGB(r*c.r, g*c.g, b*c.b);
}

RGB RGB::absorb(double dist) const
{
    return RGB(exp(-r*dist), exp(-g*dist), exp(-b*dist));
}

void RGB::min(double t)
{
    r = std::min(r, t);
    g = std::min(g, t);
    b = std::min(b, t);
}

RGB operator *(const RGB& c, const double p)
{
    return RGB(c.r*p, c.g*p, c.b*p);
}

RGB operator /(const RGB& c, const double p)
{
    return RGB(c.r/p, c.g/p, c.b/p);
}

RGB operator +(const RGB& A, const RGB& B)
{
    return RGB(A.r+B.r, A.g+B.g, A.b+B.b);
}

RGB RGB::black()
{
    return RGB(0, 0, 0);
}
RGB RGB::white()
{
    return RGB(1, 1, 1);
}
RGB RGB::red()
{
    return RGB(1, 0, 0);
}
RGB RGB::blue()
{
    return RGB(0, 0, 1);
}
RGB RGB::green()
{
    return RGB(0, 1, 0);
}
RGB RGB::zero()
{
    return RGB(0, 0, 0);
}
RGB RGB::one()
{
    return RGB(1, 1, 1);
}