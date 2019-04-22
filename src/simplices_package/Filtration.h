//
// Created by rolo on 21-01-19.
//

#ifndef PMTOOL_FILTRATION_H
#define PMTOOL_FILTRATION_H

#include <vector>
using namespace std;

#include "Simplex.h"

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


#endif //PMTOOL_FILTRATION_H
