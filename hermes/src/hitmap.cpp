#include "hitmap.hpp"
#include <algorithm>
#include <assert.h>
using namespace std;

HitMap::HitMap(int maxsize)
    : maxsize(maxsize), stored_points(0)
{
    hitpoints = new HitPoint[maxsize];
}
HitMap::~HitMap()
{
    delete[] hitpoints;
}

void HitMap::insertPoint(const CollideInfo& cinfo, int rc, RGB weight, Material::ptr material)
{
    assert(stored_points < maxsize);
    hitpoints[stored_points].pos = cinfo.p;
    hitpoints[stored_points].dir = cinfo.rayd;
    hitpoints[stored_points].n = cinfo.n;
    hitpoints[stored_points].weight = weight;
    hitpoints[stored_points].rc = rc;
    hitpoints[stored_points].color = RGB::zero();
    hitpoints[stored_points].deltaNum = 0;
    hitpoints[stored_points].material = material;
    stored_points ++;
}

void HitMap::buildTree()
{
    build(0, stored_points);
}

void HitMap::insertPhoton(const Photon& pho, const CollideInfo& cinfo, double R)
{
    update(0, stored_points, pho, cinfo, R*R);
}

void HitMap::build(int s, int t)
{
    if (s >= t) return;
    Vector boxMin, boxMax;
    boxMin = boxMax = hitpoints[s].pos;
    for(int i = s; i < t; i ++)
        for(int axis = 0; axis < 3; axis ++)
        {
            boxMin[axis] = min(boxMin[axis], hitpoints[i].pos[axis]);
            boxMax[axis] = max(boxMax[axis], hitpoints[i].pos[axis]);
        }
    double maxrange = 0;
    int index = 0;
    for(int axis = 0; axis < 3; axis ++)
        if (maxrange < boxMax[axis]-boxMin[axis])
        {
            maxrange = boxMax[axis]-boxMin[axis];
            index = axis;
        }
    sort(hitpoints+s, hitpoints+t, [index](const HitPoint& A, const HitPoint& B) {
        return dcmp(A.pos[index] - B.pos[index]) < 0;
    });
    int root = (s+t)/2;
    hitpoints[root].boxMin = boxMin;
    hitpoints[root].boxMax = boxMax;
    build(s, root); build(root+1, t);
}

void HitMap::update(int s, int t, const Photon& pho, const CollideInfo& cinfo, double R2)
{
    if (s >= t) return;
    int root = (s+t)/2;
    if (dcmp(Length2(cinfo.p-hitpoints[root].pos)-R2)<=0) {
        updateHitPoint(hitpoints[root], pho, cinfo);
    }
    double mindis2 = 0;
    for(int axis = 0; axis < 3; axis ++)
        if (cinfo.p[axis] < hitpoints[root].boxMin[axis])
            mindis2 += (hitpoints[root].boxMin[axis]-cinfo.p[axis])*(hitpoints[root].boxMin[axis]-cinfo.p[axis]);
        else if (hitpoints[root].boxMax[axis] < cinfo.p[axis])
            mindis2 += (cinfo.p[axis]-hitpoints[root].boxMax[axis])*(cinfo.p[axis]-hitpoints[root].boxMax[axis]);
    if (dcmp(mindis2-R2)>0) return;
    update(s, root, pho, cinfo, R2);
    update(root+1, t, pho, cinfo, R2);
}

void HitMap::updateHitPoint(HitPoint& point, const Photon& pho, const CollideInfo& cinfo)
{
    point.deltaNum ++;
    point.color = point.color + pho.power * point.material->BRDF(pho.movement.d, cinfo.n, -cinfo.rayd);
}