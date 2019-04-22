//
// Created by rolo on 21-01-19.
//

#ifndef PMTOOL_SIMPLEXREPOSITORY_H
#define PMTOOL_SIMPLEXREPOSITORY_H

#include "Simplex.h"
#include <vector>
using namespace std;

class SimplexRepository
{
private:
    static SimplexRepository* _instance;
    SimplexRepository();
    vector<Simplex*> ksimplexTable;
    vector<int> ksimplexRadiousCount;
public:
    static SimplexRepository* instance();
    Simplex* getSimplex(int pos);
    void addSimplex(Simplex* s);
    int getKSimplexCountByDimension(int dim);
    int setKSimplexCountByDimension(int dim, int size);
    inline int getSimplexCount() const {return ksimplexTable.size();}
    void clear();
};




#endif //PMTOOL_SIMPLEXREPOSITORY_H
