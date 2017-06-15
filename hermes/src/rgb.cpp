#include "rgb.h"
#include "const.h"

RGB::RGB(): r(0), g(0), b(0) {}
RGB::RGB(double r, double g, double b): r(r), g(g), b(b) {}

RGB RGB::modulate(const RGB& c) const
{
    return RGB(r*c.r, g*c.g, b*c.b);
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