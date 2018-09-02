//
// Created by rolo on 29-08-18.
//

#ifndef PMTOOL_BOUNDINGSPHERE_H
#define PMTOOL_BOUNDINGSPHERE_H


#include "Point.h"
#include <vector>

using namespace std;

class BoundingSphereOctree {
private:
    float radius;

    const float radiusEpsilon = 1e-4f;   // NOTE: To avoid numerical inaccuracies

    int center;
    BoundingSphereOctree* childs[8];
public:
    BoundingSphereOctree(float _radius, int _center);
    BoundingSphereOctree();
    BoundingSphereOctree(const BoundingSphereOctree &bS);
    BoundingSphereOctree(int pointO);
    BoundingSphereOctree(int pointO, int pointA);
    BoundingSphereOctree(int pO, int pA,  int pB);
    BoundingSphereOctree(int pO, int pA, int pB, int pC);
    ~BoundingSphereOctree();
    BoundingSphereOctree* BuildFromPointCloud(vector<int>* points, vector<int>* bpoints);
    BoundingSphereOctree* BuildTriangleCircumSphere(int pointA, int pointB, int PointC);
    BoundingSphereOctree* BuildTetrahedronCircumsphere(int pointA, int pointB, int PointC, int pointD);
    bool isLeaf();
    inline float getRadius() {return radius;}
    bool hasInside(BoundingSphereOctree* bso);
    bool intersects(BoundingSphereOctree* bso);
    int distanceToPoint(int point);
    BoundingSphereOctree* recurseMini(vector<int> P, unsigned int p, unsigned int b);
    BoundingSphereOctree* miniCircumSphere(vector<int> P, unsigned int p);

    BoundingSphereOctree* smallCircumSphere(vector<int> P, unsigned int p);

    BoundingSphereOctree& operator=(const BoundingSphereOctree &bS);
};


#endif //PMTOOL_BOUNDINGSPHERE_H
