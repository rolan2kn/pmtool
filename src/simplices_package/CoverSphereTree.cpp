//
// Created by rolo on 29-08-18.
//


#include "CoverSphereTree.h"
#include <cmath>
#include "../PointRepository.h"
#include "SimplexRepository.h"


CoverSphereTree::CoverSphereTree(int _radius, int _center)
: radius(_radius), center(_center), childs(), simplexTree(new SimplexTree(_center))
{

}

CoverSphereTree::CoverSphereTree() : radius(-1), center(-1), childs(), simplexTree(new SimplexTree(-1))
{
}

CoverSphereTree::CoverSphereTree(const CoverSphereTree &bS) : radius(bS.radius), center(bS.center), simplexTree(bS.simplexTree)
{
    int size = bS.childs.size();

    for(int i = 0; i < size; i++)
        this->childs.push_back(bS.childs[i]);
}

CoverSphereTree::CoverSphereTree(const vector<int>& centers, int _radious)
{
    int size = centers.size();

    CoverSphereTree* cst = new CoverSphereTree(_radious, centers[0]);

    for (int i = 1; i < size; i++)
        cst = cst->merge(new CoverSphereTree(_radious, centers[i]));

    center = cst->center;
    radius = cst->radius;

    int csize = cst->childs.size();

    for(int i = 0; i < csize; i++)
    {
        this->childs.push_back(cst->childs[i]);
    }

    simplexTree->merge(cst->getSimplexTree());

    delete cst;

}

CoverSphereTree::~CoverSphereTree()
{
    int size = this->childs.size();

    for (int i = 0; i < size; i++)
    {
        if (this->childs[i] != nullptr)
            delete this->childs[i];
    }
}

bool CoverSphereTree::isLeaf()
{
    bool noChild = false;
    int i = 0;
    int size = this->childs.size();
    while(!noChild && i < size)
    {
        noChild = (this->childs[i] != nullptr); // this check if every child is null
        i++;
    }

    return !noChild;
}

bool CoverSphereTree::hasInside(CoverSphereTree* coverSphereTree)
{
    /**
     * One sphere with radius R1 y center C1 is inside other sphere with radius R2 y Center C2
     * if and only if:
     *
     * |C2 - C1| + R1 < R2
     *
     * or |C2 - C1| <= R2-R1
     *
     *
     * **/
    if(coverSphereTree == nullptr)
        return false;

    float radius_difference = this->radius - coverSphereTree->getRadius();

    float centerSquareDistance = PointRepository::instance()->squareMagnitude(this->center, coverSphereTree->center);

    return centerSquareDistance <= (radius_difference*radius_difference);
}

bool CoverSphereTree::intersects(CoverSphereTree* coverSphereTree)
{
    /**
     * One sphere with radius R1 y center C1 intersect other sphere with radius R2 y Center C2
     * if and only if:
     *
     * |C2 - C1| < R2+R1
     * **/
    if(coverSphereTree == nullptr)
        return false;


    float centerSquareDistance = PointRepository::instance()->squareMagnitude(this->center, coverSphereTree->center);

    float radius_sum = this->radius + coverSphereTree->getRadius();

    if (centerSquareDistance <= (radius_sum*radius_sum)) // one sphere is content in other
        return true;

    return false;
}

CoverSphereTree& CoverSphereTree::operator=(const CoverSphereTree &cS)
{
    this->radius = cS.radius;
    this->center = cS.center;

    for (int i = 0; i > 4; i++)
        this->childs[i] = cS.childs[i];

    return *this;
}

CoverSphereTree* CoverSphereTree::merge(CoverSphereTree* cso)
{
    if(cso == nullptr)
        return this;

    CoverSphereTree* cst = new CoverSphereTree(radius, center);

    POINT center0 = *PointRepository::instance()->getPoint(this->center);
    POINT center1 = *PointRepository::instance()->getPoint(cso->center);

    POINT* realcenter = new POINT((center0 + center1).multiply(0.5)); //center point

    float d = center0.distance(center1);

    PointRepository::instance()->add(realcenter);

    cst->childs.push_back(this);
    cst->childs.push_back(cso);

//    cst->buildSimplex(this->getSimplexTree());
//    cst->buildSimplex(cso->getSimplexTree());

    cst->radius = int((radius + cso->radius + d)*0.5);
    cst->center = PointRepository::instance()->getPointCount()-1;

    return cst;
}

CoverSphereTree* CoverSphereTree::getChild(int i)
{
    if(i < 0 || i > childs.size())
        return nullptr;
    return childs[i];
}

SimplexTree* CoverSphereTree::getSimplexTree()
{
    return simplexTree;
}

void CoverSphereTree::buildSimplex(SimplexTree* st) {
    //TODO: ....
    /***
     * This method returns the prefix of a k-simplex that is the last simplex builded*/
     if (simplexTree == nullptr)
     {
         simplexTree = new SimplexTree(-1);
     }
    simplexTree->merge(st);
}

bool CoverSphereTree::operator== (const CoverSphereTree& cst)
{
    if(this->center != cst.center)
        return false;

    if(this->radius != cst.radius)
        return false;

    int size = this->childs.size();
    if(this->childs.size() != cst.childs.size())
        return false;

    bool areEqual = true;
    int i = 0;
    while(areEqual && i < size)
    {
        areEqual = *(this->childs[i]) == *(cst.childs[i]);
        ++i;
    }

    return areEqual;
}

int CoverSphereTree::squareDistance(const CoverSphereTree* cst)
{
    return PointRepository::instance()->squareMagnitude(this->center, cst->center);
}

std::ostream& operator<<(std::ostream& os, const CoverSphereTree* pr)
{
    return os << *pr;
}

std::ostream& operator<<(std::ostream& os, const CoverSphereTree& pr)
{
    long address = (long)&pr;
    os<<"CoverSphereTree (\naddress: "<<address<<endl;
    os<<"Center Index: "<<pr.center<<", Pos: "<<PointRepository::instance()->getPoint(pr.center)<<endl;
    os<<"Radious: "<<pr.radius<<endl;

    int size = pr.getChildsCount();

    os<<"Childs: {";

    for (int i = 0; i < size; i++)
    {
        os << "\nchild: "<< i<<": "<<pr.getChild(i);
    }

    os<<"\n}\n)";

    return os;
}