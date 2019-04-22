//
// Created by rolo on 21-01-19.
//

#include "SimplexRepository.h"


/***
 * SimplexRepository
*
 **/

SimplexRepository* SimplexRepository::_instance = nullptr;

SimplexRepository::SimplexRepository()
        : ksimplexTable(), ksimplexRadiousCount()
{

}

SimplexRepository* SimplexRepository::instance()
{
    if (SimplexRepository::_instance == nullptr)
        SimplexRepository::_instance = new SimplexRepository();

    return SimplexRepository::_instance;
}

void SimplexRepository::addSimplex(Simplex* s)
{
    ksimplexTable.push_back(s);
}

Simplex* SimplexRepository::getSimplex(int pos)
{
    if(pos > 0 && pos >= ksimplexTable.size() )
        return nullptr;

    return ksimplexTable[pos];
}

int SimplexRepository::getKSimplexCountByDimension(int pos)
{
    if(pos > 0 && pos >= ksimplexRadiousCount.size() )
        return 0;

    return ksimplexRadiousCount[pos];
}

int SimplexRepository::setKSimplexCountByDimension(int dim, int size)
{
    int sizeDim = ksimplexRadiousCount.size();

    while (sizeDim < dim+1)
    {
        ksimplexRadiousCount.push_back(0);
        ++sizeDim;
    }

    ksimplexRadiousCount[dim] = size;
}

void SimplexRepository::clear()
{
    for(Simplex* s: ksimplexTable)
    {
        if (s != nullptr)
            delete s;
    }

    ksimplexTable.clear();
    ksimplexRadiousCount.clear();
}
