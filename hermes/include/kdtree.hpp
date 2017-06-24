// 三角面片KD树inverse
#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include "const.hpp"
#include <assert.h>

struct Triangle
{
    Point pos[3];
    Vector n;
    Triangle(const Point& p0, const Point& p1, const Point& p2)
    {
        pos[0] = p0; pos[1] = p1; pos[2] = p2;
        n = Normalize(Cross(p1-p0, p2-p1));
    }
    double collide(const Ray& ray) const
    {
        // http://www.cnblogs.com/graphics/archive/2010/08/09/1795348.html

        Vector E1 = pos[1] - pos[0], E2 = pos[2] - pos[0], T = ray.s - pos[0];
        Vector P = Cross(ray.d, E2), Q = Cross(T, E1);
        double b = Dot(P, E1);
        if (dcmp(b) == 0) return -1;
        double t = Dot(Q, E2)/b, u = Dot(P, T)/b, v = Dot(Q, ray.d)/b;
        if (dcmp(t) > 0 && dcmp(u)>=0 && dcmp(u-1)<=0 && dcmp(v)>=0 && dcmp(v-1)<=0 && dcmp(u+v-1)<=0)
            return t;
        return -1;

        // Eigen::Matrix<double, 3, 3> m;
        // m(0, 0) = ray.d.x; m(0, 1) = pos[0].x-pos[1].x; m(0, 2) = pos[0].x-pos[2].x;
        // m(1, 0) = ray.d.y; m(1, 1) = pos[0].y-pos[1].y; m(1, 2) = pos[0].y-pos[2].y;
        // m(2, 0) = ray.d.z; m(2, 1) = pos[0].z-pos[1].z; m(2, 2) = pos[0].z-pos[2].z;
        // Eigen::Matrix<double, 3, 1> k;
        // k(0, 0) = pos[0].x-ray.s.x; k(1, 0) = pos[0].y-ray.s.y; k(2, 0) = pos[0].z-ray.s.z;
        
        // Eigen::Matrix<double, 3, 3> inverse;
        // bool invertible;
        // double determinant;
        // m.computeInverseAndDetWithCheck(inverse, determinant, invertible);
        // if (invertible) {
        //     auto x = inverse*k;
        //     double t = x[0], beta = x[1], gama = x[2];
        //     if (dcmp(t) > 0 && dcmp(beta)>=0 && dcmp(beta-1)<=0 && dcmp(gama)>=0 && dcmp(gama-1)<=0 && dcmp(beta+gama-1)<=0)
        //         return t;
        // }
        // return -1;
    }
};

class KDTree
{
public:
    typedef shared_ptr<KDTree> ptr;

    void loadFromObjFile(const string& filename);
    void insertTriangle(const Point& p0, const Point& p1, const Point& p2, int info);
    void buildTree();
    CollideInfo collide(const Ray& ray) const;
    int collideInfo(const Ray& ray) const;
private:
    struct Node
    {
        Triangle tri;
        Vector boxMin, boxMax;
        int info;
    };
    struct Info
    {
        double t;
        Vector n;
        int info;
    };
    vector<Node> nodes;

    void build(int s, int t);
    Info query(int s, int t, const Ray& ray) const;
};