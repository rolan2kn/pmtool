//
// Created by rolo on 20-01-19.
//.

#ifndef PMTOOL_SIMPLICES_H
#define PMTOOL_SIMPLICES_H
#include <vector>
#include <map>
using namespace std;

/**
 * This class represents a k-simplex element
 * with cardinality(o) = k + 1
 *
 * **/
class Simplex {
private:
    vector<int> elements;
public:
    Simplex();
    void addItem(int it);
    int getDimension();
    const vector<int>& start();
    const vector<int>& link();
    bool hasElement(int el);
};

class SimplexRepository
{
private:
    static SimplexRepository* instance;
    SimplexRepository();
    vector<Simplex*> ksimplexTable;
    vector<int> ksimplexRadiousCount;
public:
    static SimplexRepository* getInstance();
    Simplex* getSimplice(int pos);
    int getKSimplexCountByDimension(int dim);
};

struct SimplicialComplex
{
    int radious;
    int ksimplexidx;
    SimplicialComplex(int ks, int r): radious(r), ksimplexidx(ks){}
};

class Filtration
{
private:
    vector<SimplicialComplex*> levels;
    int maxNoLevels;
public:
    Filtration(int nolevels);
    Simplex* getSimplexInLevel(int level);
    Simplex* getSimplexFromRadious(int radious);
    int getLevelFromRadious(int radious);
    void addSimplex(int simplexid, int radious);
    inline int getMaxLevels(){ return maxNoLevels;}
    inline int getLevelsCount(){return levels.size();}
};

class SimplicialComplexBuilder
{
public:
    static Simplex* executeCech(Filtration* f, int radious);
    static Simplex* executeVR(Filtration* f, int radious);
    static Simplex* executeDelaunay(Filtration* f, int radious);
    static Simplex* executeAlpha(Filtration* f, int radious);
};

class IntersectionSphereTree
{

};

class SweepHiperPlane
{
private:
    vector<int> sortedPoints;

public:

};

#endif //PMTOOL_SIMPLICES_H
