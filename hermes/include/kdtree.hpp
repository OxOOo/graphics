// 三角面片KD树
#pragma once

#include "const.hpp"

class KDTree
{
public:
    void loadFromObjFile(string filename);
private:
    struct Node
    {
        Point p[3];
        Vector boxMin, boxMax;
    };
    vector<Node> nodes;
    vector<Point> points;

    void build(int s, int t);
};