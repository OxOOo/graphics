#include <geometry.h>

#include <set>

using namespace std;

vector<Pointi> nearlyPoints(const vector<Pointi>& target_points, int range)
{
    set<Pointi> targets;
    for(int i = 0; i < (int)target_points.size(); i ++)
    {
        targets.insert(target_points[i]);
    }

    set<Pointi> nearly_points;
    for(set<Pointi>::iterator i = targets.begin(); i != targets.end(); i ++)
    {
        const Pointi& P = *i;
        for(int k = 0; k < range; k ++)
            if (targets.find(P+ps[k]) == targets.end())
                nearly_points.insert(P+ps[k]);
    }
    
    vector<Pointi> rst;
    for(set<Pointi>::iterator i = nearly_points.begin(); i != nearly_points.end(); i ++)
    {
        rst.push_back(*i);
    }
    return rst;
}