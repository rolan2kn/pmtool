//
// Created by rkindela on 31-01-19.
//

#ifndef PMTOOL_SIMPLEXTREE_H
#define PMTOOL_SIMPLEXTREE_H

#include "Simplex.h"
#include <map>
#include <stack>

using namespace std;

class SimplexTree
{
private:
    int root;
    int level;
    map<int, SimplexTree*> siblings; //
    SimplexTree* childs;

private:
    SimplexTree* getSiblingByRoot(int _root);
    void getStarHelper(int item, vector<SimplexTree*>* StList, vector<Simplex*>* Simplexes, int currpos);

public:
    SimplexTree(int _root);
    SimplexTree(const string& filename);
    ~SimplexTree();

    void insert(const Simplex* new_simplex, int pos);
    void merge(SimplexTree* new_simplex_tree);
    inline void setLevel(int _level) {this->level = level;}

    vector<Simplex*>* getStar(Simplex* sigma);
    vector<Simplex*>* getLink(Simplex* sigma);

    inline int getRoot() const {return root;}
    inline SimplexTree* getChilds() const { return childs;}
    inline int getSiblingsCount() const {return siblings.size();}
    inline SimplexTree* getSibling(int pos) {return siblings[pos];}

    friend std::ostream& operator<<(std::ostream& os, const SimplexTree* st);
    friend std::ostream& operator<<(std::ostream& os, const SimplexTree& st);
};


#endif //PMTOOL_SIMPLEXTREE_H
