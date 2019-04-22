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
    int id;
public:
    Simplex();
    Simplex(const Simplex& s);
    void addItem(int it);
    void setId(int _id) {id = _id;}
    int getItem(int pos) const {return elements[pos];}
    int getDimension() const;
    bool hasElement(int el);
    inline int getId() {return id;}
    inline int getCard() const {return elements.size();}

    friend std::ostream& operator<<(std::ostream& os, const Simplex* st);
    friend std::ostream& operator<<(std::ostream& os, const Simplex& st);
};




#endif //PMTOOL_SIMPLICES_H
