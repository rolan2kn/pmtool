//
// Created by rolo on 29-08-18.
//

#ifndef PMTOOL_COVERSPHERETREE_H
#define PMTOOL_COVERSPHERETREE_H


#include "../Point.h"
#include <vector>
#include "Simplex.h"
#include "SimplexTree.h"

using namespace std;

class CoverSphereTree {
private:
    int radius;
    int center;
    vector<CoverSphereTree*> childs;
    SimplexTree* simplexTree;

public:
    CoverSphereTree(int _radius, int _center);
    CoverSphereTree();
    CoverSphereTree(const CoverSphereTree &cSt);
    CoverSphereTree(const vector<int>& centers, int _radious);

    ~CoverSphereTree();

    bool isLeaf();
    inline int getRadius() {return radius;}
    inline int getCenter(){return center;}
    bool hasInside(CoverSphereTree* bso);
    bool intersects(CoverSphereTree* bso);
    CoverSphereTree* merge(CoverSphereTree* cso);
    CoverSphereTree* getChild(int i);
    CoverSphereTree* getChild(int i)const {return childs[i];};
    inline  int getChildsCount() const{return childs.size();};
    int squareDistance(const CoverSphereTree* cst);
    SimplexTree* getSimplexTree();
    void buildSimplex(SimplexTree*);
    void clearChilds() {childs.clear();}

    CoverSphereTree& operator=(const CoverSphereTree &bS);
    bool operator== (const CoverSphereTree& cst);
    friend std::ostream& operator<<(std::ostream& os, const CoverSphereTree* pr);
    friend std::ostream& operator<<(std::ostream& os, const CoverSphereTree& pr);
};


#endif //PMTOOL_COVERSPHERETREE_H
