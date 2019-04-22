//
// Created by rolo on 21-01-19.
//

#include "Filtration.h"
#include "SimplexRepository.h"

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
    return SimplexRepository::instance()->getSimplex(sc->ksimplexidx);
}

Simplex* Filtration::getSimplexFromRadious(int radious)
{
    int level = this->getLevelFromRadious(radious);

    if (level == -1)
        return nullptr;

    SimplicialComplex* sc = levels[level];
    return SimplexRepository::instance()->getSimplex(sc->ksimplexidx);
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


