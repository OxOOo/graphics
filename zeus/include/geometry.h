#ifndef ZEUS_GEOMETRY_H
#define ZEUS_GEOMETRY_H

#include <cmath>
#include <vector>

const double PI = std::acos(-1);

template<typename T>
struct Point
{
    T x, y;
    Point(T x = 0, T y = 0): x(x), y(y) {}
};

typedef Point<double> Pointf;
typedef Point<int> Pointi;
typedef Point<double> Vectorf;
typedef Point<int> Vectori;

const Pointi ps[] = {Pointi(1, 0), Pointi(-1, 0), Pointi(0, 1), Pointi(0, -1), Pointi(1, 1), Pointi(1, -1), Pointi(-1, 1), Pointi(-1, -1)};

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

std::vector<Pointi> nearlyPoints(const std::vector<Pointi>& target_points, int range = 4);

#endif // ZEUS_GEOMETRY_H