//
// Created by rkindela on 31-01-19.
//

#ifndef PMTOOL_SIMPLEXTREEO_H
#define PMTOOL_SIMPLEXTREEO_H

#include "Simplex.h"
#include <map>

using namespace std;

class SimplexTreeOld
{
private:
    int root;
    vector<SimplexTreeOld*> siblings; //
    SimplexTreeOld* childs;

private:
    SimplexTreeOld* getSiblingByRoot(int _root);

public:
    SimplexTreeOld(int _root);
    ~SimplexTreeOld();

    void insert(const Simplex* new_simplex, int& pos);
    void merge(SimplexTreeOld* new_simplex_tree);

    inline int getRoot() const {return root;}
    inline SimplexTreeOld* getChilds() const { return childs;}
    inline int getSiblingsCount() const {return siblings.size();}
    inline SimplexTreeOld* getSibling(int pos) {return siblings[pos];}

    friend std::ostream& operator<<(std::ostream& os, const SimplexTreeOld* st);
    friend std::ostream& operator<<(std::ostream& os, const SimplexTreeOld& st);
};


#endif //PMTOOL_SIMPLEXTREEO_H
