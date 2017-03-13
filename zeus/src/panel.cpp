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
    std::vector<Pointi> points = linearPoints(A, B);

    for(int i = 0; i < (int)points.size(); i ++)
    {
        this->image->setPixel(points[i].x, points[i].y, pixel);
    }
}

void Panel::drawCircle(int x, int y, int r, Pixel pixel)
{
    NOT_IMPLEMENTED
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