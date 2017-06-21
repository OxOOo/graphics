#ifndef RGB_H
#define RGB_H

#include <opencv2/opencv.hpp>
#include <string>

class RGB
{
public:
    double r, g, b; // [0, 1]
    RGB();
    RGB(double r, double g, double b);
    RGB(cv::Vec3b color);
    RGB(std::string color);

    static RGB black();
    static RGB white();
    static RGB red();
    static RGB blue();
    static RGB green();
    static RGB zero();
    static RGB one();

    RGB modulate(const RGB& c) const;
    RGB absorb(double dist) const;
    void min(double t = 1);
    void print() const;
};

RGB operator *(const RGB& c, const double p);

RGB operator /(const RGB& c, const double p);

RGB operator +(const RGB& A, const RGB& B);

#endif // RGB_H