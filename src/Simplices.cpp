//
// Created by rolo on 20-01-19.
//

#include "Simplices.h"


SimplexRepository* SimplexRepository::instance = nullptr;

/**************************
*******Simplex CLass*******
***************************/

Simplex::Simplex()
:elements()
{
}

void Simplex::addItem(int it)
{
    elements.push_back(it);
}

int Simplex::getDimension()
{
    int max = 0, tmp_dim = 0;
    for(auto e: elements)
    {
        Simplex* o = SimplexRepository::getInstance()->getSimplice(e);
        tmp_dim = o->getDimension();
        if (tmp_dim > max)
            max = tmp_dim;
    }
    return max;
}

const vector<int>& Simplex::start()
{
    vector<int>* a = nullptr;
    return *a;
}

const vector<int>& Simplex::link()
{
    vector<int>* a = nullptr;
    return *a;
}

bool Simplex::hasElement(int el)
{
    for(auto e: elements)
    {
        if (e == el)
            return true;

        Simplex* o = SimplexRepository::getInstance()->getSimplice(e);
        if(o->hasElement(el))
            return true;
    }
    return false;
}

/***
 * SimplexRepository
*
 **/


SimplexRepository::SimplexRepository()
: ksimplexTable(), ksimplexDimensionCount()
{

}

SimplexRepository* SimplexRepository::getInstance()
{
    if (SimplexRepository::instance == nullptr)
        SimplexRepository::instance = new SimplexRepository();

    return SimplexRepository::instance;
}

Simplex* SimplexRepository::getSimplice(int pos)
{
    if(pos > 0 && pos >= ksimplexTable.size() )
        return nullptr;

    return ksimplexTable[pos];
}

int SimplexRepository::getKSimplexCountByDimension(int pos)
{
    if(pos > 0 && pos >= ksimplexDimensionCount.size() )
        return 0;

    return ksimplexDimensionCount[pos];
}

/***
 * Filtrations
 * *
**/

Filtration::Filtration(int nolevels)
{
    this->maxNoLevels = nolevels;
}

Simplex* Filtration::getSimplexInLevel(int level)
{
    if(level < 0 )
        level = 0;
    else if(level > maxNoLevels-1 || level > levels.size()-1)
        level = levels.size()-1;

    if(levels.size() == 0)
        return nullptr;

    SimplicialComplex* sc = levels[level];
    return SimplexRepository::getInstance()->getSimplice(sc->ksimplexidx);
}

Simplex* Filtration::getSimplexFromRadious(int radious)
{
    int level = this->getLevelFromRadious(radious);

    if (level == -1)
        return nullptr;

    SimplicialComplex* sc = levels[level];
    return SimplexRepository::getInstance()->getSimplice(sc->ksimplexidx);
}

int Filtration::getLevelFromRadious(int radious)
{
    int pos = -1;

    for (auto level: levels)
    {
        ++pos;
        if(level->radious == radious)
            return pos;
    }

    return -1;
}

void Filtration::addSimplex(int simplexid, int radious)
{
    int pos = getLevelFromRadious(radious);

    if (pos != -1)
    {
        SimplicialComplex* sc = levels[pos];
        sc->ksimplexidx = simplexid;
    }
    else
    {
        levels.push_back(new SimplicialComplex(simplexid, radious));
    }
}

/***
 * SimplicialComplexBuilder
 */

Simplex* SimplicialComplexBuilder::executeCech(Filtration* f, int radious)
{

}

Simplex* SimplicialComplexBuilder::executeVR(Filtration* f, int radious)
{

}

Simplex* SimplicialComplexBuilder::executeDelaunay(Filtration* f, int radious)
{

}

Simplex* SimplicialComplexBuilder::executeAlpha(Filtration* f, int radious)
{

}
