// Bezier
#pragma once

#include "const.hpp"

// 绕Z轴旋转
class Bezier
{
public:
    typedef shared_ptr<Bezier> ptr;

    void append(const Point& p);
    void maintain();
    Point P(double t) const; // 点坐标
    Point P(double t, double theta) const;
    Vector Pt(double t) const; // 斜率
    cv::Mat draw(int size = 200) const;
    CollideInfo collide(const Ray& ray) const;
private:
    vector<Point> points;
    Point box_center;
    double box_radius2;
};

template<typename T>
class DMatrix
{
private:
    T* content;
    T** ptr;
public:
    DMatrix(int n, int m) {
        content = new T[n*m];
        ptr = new T*[n];
        for(int i = 0; i < n; i ++)
            ptr[i] = content + i*m;
    }
    ~DMatrix() {
        delete[] content;
        delete[] ptr;
    }
    T* operator[](int x) {
        return ptr[x];
    }
    const T* operator[](int x) const {
        return ptr[x];
    }
};