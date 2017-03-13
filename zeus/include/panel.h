#ifndef ZEUS_PANEL_H
#define ZEUS_PANEL_H

#include <image.h>
#include <geometry.h>

#include <string>
#include <vector>

// 二维世界，坐标和图片无关
class Panel
{
public:
    Panel(int height, int width);
    ~Panel();

    /// 画线
    void drawLine(int x1, int y1, int x2, int y2, Pixel pixel);
    /// 画圆
    void drawCircle(int x, int y, int r, Pixel pixel);
    /// 画多边形
    void drawPolygon(std::vector<Pointi> points, Pixel pixel);

    void save(const std::string& filename);

private:
    Image::ptr image;

    std::vector<Pointi> linearPoints(Pointi A, Pointi B, bool y = false);
    std::vector<Pointi> circlePoints(Pointi C, int r);
};

#endif // ZEUS_PANEL_H