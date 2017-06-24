#include "kdtree.hpp"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;

void KDTree::loadFromObjFile(const string& filename)
{
    vector<Point> points;
    nodes.clear();

    ifstream fin(filename);
    string buf;
    while(getline(fin, buf, '\n')) {
        stringstream line(buf);
        string cmd;
        if (!(line >> cmd)) continue;
        if (cmd == "v") {
            Point p;
            line >> p.x >> p.y >> p.z;
            points.push_back(p);
        } else if (cmd == "f") {
            int a, b, c;
            line >> a >> b >> c;
            Triangle tri(points[a-1], points[b-1], points[c-1]);
            if (dcmp(Length2(tri.n)-1)==0)
                nodes.push_back((Node){tri, Vector(), Vector(), 0});
        }
    }
}

void KDTree::insertTriangle(const Point& p0, const Point& p1, const Point& p2, int info)
{
    Triangle tri(p0, p1, p2);
    if (dcmp(Length2(tri.n)-1)==0)
        nodes.push_back((Node){tri, Vector(), Vector(), info});
}

void KDTree::buildTree()
{
    build(0, nodes.size());
}

CollideInfo KDTree::collide(const Ray& ray) const
{
    Info info = query(0, nodes.size(), ray);
    if (dcmp(info.t)>0)
    {
        return (CollideInfo){info.t, ray.s+ray.d*info.t, info.n, ray.d};
    }
    return NoCollide;
}

int KDTree::collideInfo(const Ray& ray) const
{
    Info info = query(0, nodes.size(), ray);
    if (dcmp(info.t)>0)
    {
        return info.info;
    }
    return -1;
}

void KDTree::build(int s, int t)
{
    if (s == t) return;
    Vector boxMin, boxMax;
    boxMin = boxMax = nodes[s].tri.pos[0];
    for(int i = s; i < t; i ++)
        for(int axis = 0; axis < 3; axis ++)
            for(int k = 0; k < 3; k ++)
            {
                boxMin[axis] = min(boxMin[axis], nodes[i].tri.pos[k][axis]);
                boxMax[axis] = max(boxMax[axis], nodes[i].tri.pos[k][axis]);
            }
    double maxrange = 0;
    int index = 0;
    for(int axis = 0; axis < 3; axis ++)
        if (maxrange < boxMax[axis]-boxMin[axis])
        {
            maxrange = boxMax[axis]-boxMin[axis];
            index = axis;
        }
    sort(nodes.begin()+s, nodes.begin()+t, [index](const Node& A, const Node& B) {
        return dcmp(A.tri.pos[0][index] - B.tri.pos[0][index]) < 0;
    });
    int root = (s+t)/2;
    nodes[root].boxMin = boxMin;
    nodes[root].boxMax = boxMax;
    build(s, root); build(root+1, t);
}

KDTree::Info KDTree::query(int s, int t, const Ray& ray) const
{
    if (s == t) return (Info){-1, Vector()};
    int root = (s+t)/2;
    double tmin = -INF, tmax = INF;
    double diss2 = 0;
    for(int axis = 0; axis < 3; axis ++)
        if (ray.s[axis] < nodes[root].boxMin[axis])
            diss2 += (nodes[root].boxMin[axis]-ray.s[axis])*(nodes[root].boxMin[axis]-ray.s[axis]);
        else if (nodes[root].boxMax[axis] < ray.s[axis])
            diss2 += (ray.s[axis]-nodes[root].boxMax[axis])*(ray.s[axis]-nodes[root].boxMax[axis]);
    bool axisline = false;
    for(int axis = 0; axis < 3; axis ++)
    {
        Vector N;
        N[axis] = 1;
        double t1 = CollideWithSurface(ray, N, -Dot(N, nodes[root].boxMin));
        double t2 = CollideWithSurface(ray, N, -Dot(N, nodes[root].boxMax));
        if (t1 > t2) swap(t1, t2);
        if (dcmp(t1+1)==0) axisline = true;
        tmin = max(tmin, t1);
        tmax = min(tmax, t2);
    }
    bool possible = dcmp(tmin - tmax) <= 0 && dcmp(tmax) > 0;
    possible |= axisline && dcmp(diss2) == 0;
    if (!possible) return (Info){-1, Vector()};
    Info ret;
    ret.t = nodes[root].tri.collide(ray);
    ret.info = nodes[root].info;
    ret.n = dcmp(Dot(ray.d, nodes[root].tri.n))>0?-nodes[root].tri.n:nodes[root].tri.n;
    Info linfo = query(s, root, ray);
    Info rinfo = query(root+1, t, ray);
    if (dcmp(linfo.t)>0 && (dcmp(ret.t)<=0 || dcmp(linfo.t-ret.t)<0)) ret = linfo;
    if (dcmp(rinfo.t)>0 && (dcmp(ret.t)<=0 || dcmp(rinfo.t-ret.t)<0)) ret = rinfo;
    return ret;
}