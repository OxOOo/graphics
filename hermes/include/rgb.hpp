#ifndef RGB_H
#define RGB_H

class RGB
{
public:
    double r, g, b; // [0, 1]
    RGB();
    RGB(double r, double g, double b);

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
};

RGB operator *(const RGB& c, const double p);

RGB operator /(const RGB& c, const double p);

RGB operator +(const RGB& A, const RGB& B);

#endif // RGB_H