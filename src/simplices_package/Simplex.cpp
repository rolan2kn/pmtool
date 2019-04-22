//
// Created by rolo on 20-01-19.
//

#include "Simplex.h"
#include "../PointRepository.h"
#include "SimplexRepository.h"

#include <iostream>

using namespace std;



/**************************
******* Simplex CLass *******
***************************/

Simplex::Simplex()
:elements(), id(-1)
{
}

Simplex::Simplex(const Simplex& s)
:elements(), id(-1)
{
    for(auto item: s.elements)
    {
        elements.push_back(item);
    }

}

void Simplex::addItem(int it)
{

    elements.push_back(it);
}

int Simplex::getDimension() const
{
    int max = 0, tmp_dim = 0;
    for(auto e: elements)
    {
        Simplex* o = SimplexRepository::instance()->getSimplex(e);
        tmp_dim = o->getDimension();
        if (tmp_dim > max)
            max = tmp_dim;
    }
    return max;
}

bool Simplex::hasElement(int el)
{
    for(auto e: elements)
    {
        if (e == el)
            return true;

        Simplex* o = SimplexRepository::instance()->getSimplex(e);
        if(o->hasElement(el))
            return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Simplex* splx)
{
    return os<<*splx;
}

std::ostream& operator<<(std::ostream& os, const Simplex& splx)
{
    /**
     * vector<int> elements;
    int id;*/
    long address = (long)&splx;
    os<<"=> SIMPLEX (\naddress: "<<address<<endl;

    os<<"==> ID: "<<splx.id <<"\n==> ELEMENT: (";

    int size = splx.elements.size();
    if (size > 0)
        os<<splx.elements[0];
    for (int i = 1; i < size; i++)
        os<<", "<<splx.elements[i];

    os<<")";

    return os;
}

