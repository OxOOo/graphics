// 采样点
#pragma once

#include "const.hpp"
#include "rgb.hpp"
#include "material.hpp"

struct HitPoint
{
    Point pos;
    Vector dir, n;
    RGB weight;
    int rc;
    Vector boxMin, boxMax;
    int deltaNum;
    Material::ptr material;
    RGB color; // 颜色统计，考虑BRDF，不考虑材质
};

class HitMap
{
private:
    HitPoint* hitpoints;
    int maxsize;
    int stored_points;
public:
    HitMap(int maxsize);
    ~HitMap();
    typedef shared_ptr<HitMap> ptr;

    void insertPoint(const CollideInfo& cinfo, int rc, RGB weight, Material::ptr material);
    void buildTree();
    HitPoint* getHitPoints() {return hitpoints;}
    int getStoredPoints() {return stored_points;}
    void insertPhoton(const Photon& pho, const CollideInfo& cinfo, double R);
private:
    void build(int s, int t);
    void update(int s, int t, const Photon& pho, const CollideInfo& cinfo, double R2);
    void updateHitPoint(HitPoint& point, const Photon& pho, const CollideInfo& cinfo);
};