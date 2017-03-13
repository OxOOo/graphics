#ifndef ZEUS_GEOMETRY_H
#define ZEUS_GEOMETRY_H

#include <cmath>

const double PI = std::acos(-1);

template<typename T>
struct Point
{
    T x, y;
    Point(T x = 0, T y = 0): x(x), y(y) {}
};

template<typename T>
Point<T> operator+(const Point<T>&A, const Point<T>&B)
{
    return Point<T>(A.x+B.x, A.y+B.y);
}

template<typename T>
Point<T> operator-(const Point<T>&A, const Point<T>&B)
{
    return Point<T>(A.x-B.x, A.y-B.y);
}

template<typename T>
Point<T> operator*(const Point<T>&A, T p)
{
    return Point<T>(A.x*p, A.y*p);
}

template<typename T>
Point<T> operator/(const Point<T>&A, T p)
{
    return Point<T>(A.x/p, A.y/p);
}

template<typename T>
bool operator<(const Point<T>&A, const Point<T>&B)
{
    return A.x < B.x || (A.x == B.x && A.y < B.y);
}

template<typename T>
bool operator>(const Point<T>&A, const Point<T>&B)
{
    return B < A;
}

template<typename T>
bool operator==(const Point<T>&A, const Point<T>&B)
{
    return A.x == B.x && A.y == B.y;
}

template<typename T>
bool operator<=(const Point<T>&A, const Point<T>&B)
{
    return A < B || A == B;
}

template<typename T>
bool operator>=(const Point<T>&A, const Point<T>&B)
{
    return A > B || A == B;
}

template<typename T>
T cross(const Point<T>&A, const Point<T>&B)
{
    return A.x*B.y-A.y*B.x;
}

typedef Point<double> Pointf;
typedef Point<int> Pointi;
typedef Point<double> Vectorf;
typedef Point<int> Vectori;

#endif // ZEUS_GEOMETRY_H