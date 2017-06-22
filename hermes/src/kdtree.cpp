#include "kdtree.hpp"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;

void KDTree::loadFromObjFile(string filename)
{
    points.clear();
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
            
        }
    }
}