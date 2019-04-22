//
// Created by rolo on 29-08-18.
//

#ifndef PMTOOL_BOUNDINGSPHERE_H
#define PMTOOL_BOUNDINGSPHERE_H


#include "Point.h"
#include <vector>

using namespace std;

class BoundingSphereTree {
private:
    float radius;

    const float radiusEpsilon = 1e-4f;   // NOTE: To avoid numerical inaccuracies

    int center;
    BoundingSphereTree* childs[4];         // for future use, for fast computation of operations between bounding sphere trees
public:
    BoundingSphereTree(float _radius, int _center);
    BoundingSphereTree();
    BoundingSphereTree(const BoundingSphereTree &bS);
    BoundingSphereTree(int pointO);
    BoundingSphereTree(int pointO, int pointA);
    BoundingSphereTree(int pO, int pA,  int pB);
    BoundingSphereTree(int pO, int pA, int pB, int pC);
    ~BoundingSphereTree();
    BoundingSphereTree* buildFromPointCloud(vector<int> *points);
    bool isLeaf();
    inline float getRadius() {return radius;}
    bool hasInside(BoundingSphereTree* bso);
    bool intersects(BoundingSphereTree* bso);
    int distanceToPoint(int point);
    BoundingSphereTree* recurseMini(vector<int> P, unsigned int p, unsigned int b);
    BoundingSphereTree* miniCircumSphere(vector<int> P, unsigned int p);

    BoundingSphereTree* smallCircumSphere(vector<int> P, unsigned int p);

    BoundingSphereTree& operator=(const BoundingSphereTree &bS);
};


#endif //PMTOOL_BOUNDINGSPHERE_H
