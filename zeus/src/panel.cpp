#include <image.h>
#include <panel.h>
#include <common.h>

#include <cstdlib>
#include <cmath>

using namespace std;

Panel::Panel(int height, int width)
{
    this->image = Image::create(height, width);
}

Panel::~Panel()
{

}

void Panel::drawLine(int x1, int y1, int x2, int y2, Pixel pixel)
{
    Pointi A(x1, y1);
    Pointi B(x2, y2);
    std::vector<Pointi> pixels = linearPoints(A, B);

    for(int i = 0; i < (int)pixels.size(); i ++)
    {
        this->image->setPixel(pixels[i].x, pixels[i].y, pixel);
    }
}

void Panel::drawCircle(int x, int y, int r, Pixel pixel)
{
    Pointi C(x, y);
    std::vector<Pointi> pixels = circlePoints(C, r);

    for(int  i = 0; i < (int)pixels.size(); i ++)
    {
        this->image->setPixel(pixels[i].x, pixels[i].y, pixel);
    }
}

void Panel::drawPolygon(std::vector<Pointi> points, Pixel pixel)
{
    ASSERT(points.size() > 0)
    int x_min = points[0].x, x_max = points[0].x;
    int y_min = points[0].y, y_max = points[0].y;
    for(int i = 0; i < (int)points.size(); i ++)
    {
        x_min = min(x_min, points[i].x);
        x_max = max(x_max, points[i].x);
        y_min = min(y_min, points[i].y);
        y_max = max(y_max, points[i].y);
    }
    for(int x = x_min; x <= x_max; x ++)
        for(int y = y_min; y <= y_max; y ++)
        {
            int flag = 0;
            for(int i = 0; i < (int)points.size(); i ++)
            {
                const Pointi& A = points[i];
                const Pointi& B = points[(i+1)%points.size()];
                const Pointi P(x, y);

                if (min(A.y, B.y) > P.y || max(A.y, B.y) < P.y) continue;
                if (A.y > B.y) {
                    if (P.y != A.y && cross(P-A, B-A) >= 0) flag ^= 1;
                } else if (A.y < B.y) {
                    if (P.y != B.y && cross(B-A, P-A) >= 0) flag ^= 1;
                }
            }
            if(flag) this->image->setPixel(x, y, pixel);
        }
}

void Panel::save(const std::string& filename)
{
    this->image->save(filename);
}

std::vector<Pointi> Panel::linearPoints(Pointi A, Pointi B, bool y)
{
    // FIXME 抗锯齿
    // if (abs(A.x-B.x) < abs(A.y-B.y))
    // {
    //     std::vector<Pointi> rst = linearPoints(Pointi(A.y, A.x), Pointi(B.y, B.x));
    //     for(int i = 0; i < (int)rst.size(); i ++)
    //         swap(rst[i].x, rst[i].y);
    //     return rst;
    // }
    if(A > B) swap(A, B);

    std::vector<Pointi> rst;
    if(A.x != B.x)
    {
        for(int x = A.x; x <= B.x; x ++)
        {
            double y = A.y+(B.y-A.y)*double(x-A.x)/double(B.x-A.x);
            rst.push_back(Pointi(x, round(y)));
        }
    }
    if(!y)
    {
        std::vector<Pointi> y_rst = linearPoints(Pointi(A.y, A.x), Pointi(B.y, B.x), true);
        for(int i = 0; i < (int)y_rst.size(); i ++)
            rst.push_back(Pointi(y_rst[i].y, y_rst[i].x));
    }
    return rst;
}

std::vector<Pointi> Panel::circlePoints(Pointi C, int r)
{
    const int n = round(2*2*PI*r);
    std::vector<Pointi> rst;

    for(int i = 0; i < n; i ++)
    {
        rst.push_back(Pointi(round(C.x + r*cos(2*PI*i/n)), round(C.y + r*sin(2*PI*i/n))));
    }

    return rst;
}