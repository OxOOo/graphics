#include <image.h>
#include <panel.h>
#include <common.h>

#include <cstdlib>
#include <cmath>

using namespace std;

Panel::Panel(int height, int width, bool AA): AA(AA)
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
    std::vector<Pointi> target_points = linearPoints(A, B);

    for(int i = 0; i < (int)target_points.size(); i ++)
    {
        this->image->setPixel(target_points[i].x, target_points[i].y, pixel);
    }
    processAA(target_points);
}

void Panel::drawCircle(int x, int y, int r, Pixel pixel)
{
    Pointi C(x, y);
    std::vector<Pointi> target_points = circlePoints(C, r);

    for(int  i = 0; i < (int)target_points.size(); i ++)
    {
        this->image->setPixel(target_points[i].x, target_points[i].y, pixel);
    }
    processAA(target_points);
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

    std::vector<Pointi> target_points;
    for(int x = x_min; x <= x_max; x ++)
    {
        std::vector<int> list;
        for(int i = 0; i < (int)points.size(); i ++)
        {
            const Pointi& A = points[i];
            const Pointi& B = points[(i+1)%points.size()];

            if (A.x < B.x && A.x <= x && x < B.x) {
                list.push_back(A.y + round(double(B.y - A.y) * double(x - A.x) / double(B.x - A.x)));
            } else if (A.x > B.x && A.x > x && x >= B.x) {
                list.push_back(A.y + round(double(B.y - A.y) * double(x - A.x) / double(B.x - A.x)));
            }
        }
        if (list.size() > 0)
        {
            assert(list.size() % 2 == 0);
            sort(list.begin(), list.end());
            for(int i = 0; i < (int)list.size(); i += 2)
            {
                for(int y = list[i]; y <= list[i+1]; y ++)
                {
                    this->image->setPixel(x, y, pixel);
                    target_points.push_back(Pointi(x, y));
                }
            }
        }
    }
    // for(int x = x_min; x <= x_max; x ++)
    //     for(int y = y_min; y <= y_max; y ++)
    //     {
    //         int flag = 0;
    //         for(int i = 0; i < (int)points.size(); i ++)
    //         {
    //             const Pointi& A = points[i];
    //             const Pointi& B = points[(i+1)%points.size()];
    //             const Pointi P(x, y);

    //             if (min(A.y, B.y) > P.y || max(A.y, B.y) < P.y) continue;
    //             if (A.y > B.y) {
    //                 if (P.y != A.y && cross(P-A, B-A) >= 0) flag ^= 1;
    //             } else if (A.y < B.y) {
    //                 if (P.y != B.y && cross(B-A, P-A) >= 0) flag ^= 1;
    //             }
    //         }
    //         if(flag) {
    //             this->image->setPixel(x, y, pixel);
    //             target_points.push_back(Pointi(x, y));
    //         }
    //     }
    processAA(target_points);
}

void Panel::save(const std::string& filename)
{
    this->image->save(filename);
}

std::vector<Pointi> Panel::linearPoints(Pointi A, Pointi B, bool y)
{
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

void Panel::processAA(const std::vector<Pointi>& target_points)
{
    if (!AA) return;
    std::vector<Pointi> nearly_points = nearlyPoints(target_points);
    std::vector<Pixel> pixels;
    pixels.resize(nearly_points.size());

    for(int i = 0; i < (int)nearly_points.size(); i ++)
    {
        const Pointi& P = nearly_points[i];
        if (P.x < 0 || P.x >= image->height() || P.y < 0 || P.y >= image->width()) continue;

        int total = AA_CENTER_W;
        int r = image->getPixel(P.x, P.y)->r * AA_CENTER_W;
        int g = image->getPixel(P.x, P.y)->g * AA_CENTER_W;
        int b = image->getPixel(P.x, P.y)->b * AA_CENTER_W;
        int a = image->getPixel(P.x, P.y)->a * AA_CENTER_W;
        for(int k = 0; k < AA_RANGE; k ++)
        {
            const Pointi X = P + ps[k];
            if (X.x < 0 || X.x >= image->height() || X.y < 0 || X.y >= image->width()) continue;
            total ++;
            r += image->getPixel(X.x, X.y)->r;
            g += image->getPixel(X.x, X.y)->g;
            b += image->getPixel(X.x, X.y)->b;
            a += image->getPixel(X.x, X.y)->a;
        }
        pixels[i].r = r / total;
        pixels[i].g = g / total;
        pixels[i].b = b / total;
        pixels[i].a = a / total;
    }
    for(int i = 0; i < (int)nearly_points.size(); i ++)
    {
        image->setPixel(nearly_points[i].x, nearly_points[i].y, pixels[i]);
    }
}