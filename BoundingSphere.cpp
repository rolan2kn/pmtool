//
// Created by rolo on 29-08-18.
//

#include "BoundingSphere.h"
#include <cmath>
#include "PointRepository.h"
#include "Matrix.h"

BoundingSphereOctree::BoundingSphereOctree(float _radius, int _center)
: radius(_radius), center(_center)
{
    for (int i = 0; i < 8; i++)
        this->childs[i] = nullptr;
}

BoundingSphereOctree::BoundingSphereOctree() : radius(-1), center(-1)
{
    for (int i = 0; i < 8; i++)
        this->childs[i] = nullptr;
}

BoundingSphereOctree::BoundingSphereOctree(const BoundingSphereOctree &bS) : radius(bS.radius), center(bS.center)
{
    for (int i = 0; i < 8; i++)
        this->childs[i] = bS.childs[i];
}

BoundingSphereOctree::BoundingSphereOctree(int pointO)
{
    radius = 0 + radiusEpsilon;
    center = pointO;
}

BoundingSphereOctree::BoundingSphereOctree(int pointO, int pointA)
{
//    Vector3D a = A - O;
    Vector3D a = *PointRepository::instance()->getVector3dFromPoints(pointA, pointO);

    Vector3D o = a.multi(0.5f);

    this->radius = o.norm() + radiusEpsilon;
    int pos = PointRepository::instance()->getPointCount();
    POINT* pp = PointRepository::instance()->getPoint(pointO);
    POINT _center = (*pp) + o.asPoint();
    PointRepository::instance()->add(new POINT(_center));
    this->center = pos;
}

BoundingSphereOctree::BoundingSphereOctree(int pO, int pA,  int pB)
{
    Vector3D a = *PointRepository::instance()->getVector3dFromPoints(pA, pO);
    Vector3D b = *PointRepository::instance()->getVector3dFromPoints(pB, pO);

    Vector3D _cross = a.cross(b);
    float Denominator = 2.0f * (_cross).dot(_cross);

    Vector3D _axb = a.cross(b);
    Vector3D _axbxa = _cross.cross(a);
    Vector3D _bxaxb = _cross.cross(a);
    float b2 = b^2;
    float a2 = a^2;
    Vector3D o = ((_axbxa.multi(b2)) + (_bxaxb.multi(a2))) / Denominator;

    radius = o.norm() + radiusEpsilon;

    int pos = PointRepository::instance()->getPointCount();
    POINT* pp = PointRepository::instance()->getPoint(pO);
    POINT _center = (*pp) + o.asPoint();
    PointRepository::instance()->add(new POINT(_center));
    this->center = pos;
}

BoundingSphereOctree::BoundingSphereOctree(int pO, int pA, int pB, int pC)
{
    Vector3D a = *PointRepository::instance()->getVector3dFromPoints(pA, pO);
    Vector3D b = *PointRepository::instance()->getVector3dFromPoints(pB, pO);
    Vector3D c = *PointRepository::instance()->getVector3dFromPoints(pC, pO);

    float Denominator = 2.0f * Matrix::det(a.x, a.y, a.z,
                                           b.x, b.y, b.z,
                                           c.x, c.y, c.z);

    Vector3D _axb = a.cross(b);
    Vector3D _cxa = c.cross(a);
    Vector3D _bxc = b.cross(c);

    Vector3D o = ((c^2) * (_axb) +
                (b^2) * (_cxa) +
                (a^2) * (_bxc)) / Denominator;

    radius = o.norm() + radiusEpsilon;

    int pos = PointRepository::instance()->getPointCount();
    POINT* pp = PointRepository::instance()->getPoint(pO);
    POINT _center = (*pp) + o.asPoint();
    PointRepository::instance()->add(new POINT(_center));

    this->center = pos;
}

BoundingSphereOctree* BoundingSphereOctree::recurseMini(vector<int> P, unsigned int p, unsigned int b)
{
    BoundingSphereOctree* MB;
    int pcount = P.size();

    switch(b)
    {
        case 0:
            MB = new BoundingSphereOctree();
            break;
        case 1:
            MB = new BoundingSphereOctree(P[pcount-1]);
            break;
        case 2:
            MB = new BoundingSphereOctree(P[pcount-1], P[pcount-2]);
            break;
        case 3:
            MB = new BoundingSphereOctree(P[pcount-1], P[pcount-2], P[pcount-3]);
            break;
        case 4:
            MB = new BoundingSphereOctree(P[pcount-1], P[pcount-2], P[pcount-3], P[pcount-4]);
            return MB;
    }

//    for(unsigned int i = 0; i < p; i++)
//        if(MB.distanceToPoint(P[i]) > 0)   // Signed square distance to sphere
//        {
//            for(unsigned int j = i; j > 0; j--)
//            {
//                Point *T = P[j];
//                P[j] = P[j - 1];
//                P[j - 1] = T;
//            }
//
//            MB = recurseMini(P + 1, i, b + 1);
//        }

    return MB;
}

int BoundingSphereOctree::distanceToPoint(int point)
{
    return PointRepository::instance()->squareMagnitude(this->center, point);
}

BoundingSphereOctree* BoundingSphereOctree::miniCircumSphere(vector<int> P, unsigned int p)
{
    BoundingSphereOctree* MB = recurseMini(P, p, p);

    return MB;
}

BoundingSphereOctree* BoundingSphereOctree::smallCircumSphere(vector<int> P, unsigned int p)
{
    int center;
    float radius = -1;

    if(p > 0)
    {
        center = PointRepository::instance()->getCentralPointPosition(P, p);

        for(int i = 0; i < p; i++)
        {
            int d2 = PointRepository::instance()->squareMagnitude(P[i], center);

            if(d2 > radius)
                radius = d2;
        }

        radius = sqrtf(radius) + radiusEpsilon;
    }

    return new BoundingSphereOctree(center, radius);
}

BoundingSphereOctree* BoundingSphereOctree::BuildFromPointCloud(vector<int>* points, vector<int>* bpoints)
{
    /**
     * Based on https://gamedev.stackexchange.com/questions/162731/welzl-algorithm-to-find-the-smallest-bounding-sphere
     * */

    return nullptr;
}

BoundingSphereOctree::~BoundingSphereOctree()
{
    //todo
    if (this->isLeaf())
        return;

    for (int i = 0; i < 8; i++)
    {
        if (this->childs[i] != nullptr)
            delete this->childs[i];
    }

}

bool BoundingSphereOctree::isLeaf()
{
    bool noChild = false;
    int i = 0;
    while(!noChild && i < 8)
    {
        noChild = (this->childs[i] != nullptr); // this check if every child is null
        i++;
    }

    return !noChild;
}

bool BoundingSphereOctree::hasInside(BoundingSphereOctree* boundingSphereOctree)
{
    /**
     * One sphere with radius R1 y center C1 is inside other sphere with radius R2 y Center C2
     * if and only if:
     *
     * |C2 - C1| + R1 < R2
     *
     * or |C2 - C1| < R2-R1
     *
     *
     * **/
    if(boundingSphereOctree == nullptr)
        return false;

    float radius_difference = this->radius - boundingSphereOctree->getRadius();

    float centerSquareDistance = PointRepository::instance()->squareMagnitude(this->center, boundingSphereOctree->center);

    return centerSquareDistance < (radius_difference*radius_difference);
}

bool BoundingSphereOctree::intersects(BoundingSphereOctree* boundingSphereOctree)
{
    /**
     * One sphere with radius R1 y center C1 is inside other sphere with radius R2 y Center C2
     * if and only if:
     *
     * |C2 - C1| + R1 < R2
     *
     * or |C2 - C1| < R2-R1
     *
     *
     * **/
    if(boundingSphereOctree == nullptr)
        return false;

    float radius_difference = this->radius - boundingSphereOctree->getRadius();

    float centerSquareDistance = PointRepository::instance()->squareMagnitude(this->center, boundingSphereOctree->center);

    if (centerSquareDistance < (radius_difference*radius_difference)) // one sphere is content in other
        return true;

    float radius_sum = this->radius + boundingSphereOctree->getRadius();


}

BoundingSphereOctree& BoundingSphereOctree::operator=(const BoundingSphereOctree &bS)
{
    this->radius = bS.radius;
    this->center = bS.center;

    for (int i = 0; i > 8; i++)
        this->childs[i] = bS.childs[i];

    return *this;
}