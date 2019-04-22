//
// Created by rkindela on 31-01-19.
//

#include "SimplexTree.h"
#include <iostream>
#include <fstream>
#include<string>

using namespace std;


SimplexTree::SimplexTree(int _root)
: root(_root), siblings(), childs(nullptr)
{

}

SimplexTree::SimplexTree(const string& filename)
        : root(-1), siblings(), childs(nullptr)
{
    ifstream in = ifstream(filename);
    string str;
    string num;
    Simplex* smplx = nullptr;
    while(getline(in, str))
    {
        for (auto s: str)
        {
            if(s == '[')
            {
                smplx = new Simplex();
            }
            else if (s == ']')
            {
                smplx->addItem(atoi(num.c_str()));
                num = string();
                this->insert(smplx, 0);
                delete smplx;

            }
            else if(s != '\n' && s != '\r' && s != '\f' && s != ' ')
            {
                if (s == ',')
                {
                    smplx->addItem(atoi(num.c_str()));
                    num = string();
                }
                else
                {
                    num += s;
                }
            }
        }
    }
    in.close();
}

SimplexTree::~SimplexTree()
{
    if(childs != nullptr) // we destroy all childs first
    {
        delete childs;
    }

    map<int, SimplexTree*>::iterator begin = siblings.begin(), end = siblings.end();

    while(begin != end) // then we destroy all siblings
    {
        delete begin->second;
        ++begin;
    }
    siblings.clear();      // finally we clear the map
}

void SimplexTree::insert(const Simplex* new_simplex, int pos)
{
    int simplexCard = new_simplex->getCard();
    if (pos < -1 || pos+1 > simplexCard )
        return;

   int item = new_simplex->getItem(pos);

   SimplexTree* cursor = this->getSiblingByRoot(item);

   if(pos == simplexCard-1)                     // if we consume the list we are done
      return;

   if(cursor->childs == nullptr)                // if we don't have childs
   {
      item = new_simplex->getItem(pos+1);      // we get the next element
      cursor->childs = new SimplexTree(item);  // and create a child with it
   }

   cursor->childs->insert(new_simplex, ++pos);      // finally we find for insection
}

SimplexTree* SimplexTree::getSiblingByRoot(int item)
{
    if (root == item)
        return this;

    if(this->siblings.count(item))
        return this->siblings[item];

    auto result = this->siblings.insert(make_pair(item, new SimplexTree(item)));

    return this->siblings[item];
}

void SimplexTree::merge(SimplexTree* new_simplex_tree)
{
    if (new_simplex_tree == nullptr)
        return;

    SimplexTree* cursor = this->getSiblingByRoot(new_simplex_tree->getRoot()); // we found the new root between siblings

    map<int, SimplexTree*>::iterator iter = new_simplex_tree->siblings.begin();
    map<int, SimplexTree*>::iterator end = new_simplex_tree->siblings.end();

    while (iter != end)
    {
        SimplexTree* st = iter->second;                      // and for each one
        SimplexTree* tmp = cursor->getSiblingByRoot(st->getRoot());             // we found the sibling of the detected sibling
        tmp->merge(st); // and merge it
        ++iter;
    }

    SimplexTree* ochild = new_simplex_tree->getChilds();                        // finally we process the son

    if (ochild == nullptr)
        return;

    if(cursor->childs == nullptr)                                               // if we don't have childs
    {
        cursor->childs = new SimplexTree(ochild->getRoot());                        // we create one child with same root
    }
    cursor->childs->merge(ochild);                                              // finally we merge it both
}


void SimplexTree::getStarHelper(int item, vector<SimplexTree*>* StList, vector<Simplex*>* Simplexes, int currpos)
{
    Simplex* smplx = nullptr;

    if (currpos < 0)
    {
        Simplexes->push_back(new Simplex());
    }

    if (currpos < Simplexes->size())
    {
        smplx = (*Simplexes)[currpos];
    }

    if (root == item)
    {
        StList->push_back(this);
        smplx->addItem(root);
    }
    else if (this->childs != nullptr)
    {
        smplx->addItem(root);
        this->childs->getStarHelper(item, StList, Simplexes, currpos);
    }

    for (auto it = this->siblings.begin(); it != this->siblings.end(); ++it)
    {
        it->second->getStarHelper(item, StList, Simplexes, currpos);
    }
}

vector<Simplex*>* SimplexTree::getStar(Simplex* sigma)
{
    if (sigma == nullptr)
        return nullptr;

    vector<Simplex*>* simplexes = new vector<Simplex*>();
    vector<SimplexTree*>* StList = new vector<SimplexTree*>();
    int size = sigma->getCard(), i = 0, currentSt = -1, lastItSt = -1, lastStList = 0;
    SimplexTree* stTemp = this;

    vector<int> sizeList;
    sizeList.push_back(0);

    int tau = sigma->getItem(0);
    lastStList = StList->size();
    stTemp->getStarHelper(tau, StList, simplexes, currentSt);
    sizeList[i] += (StList->size() - lastStList);
    ++currentSt;

    if(StList->size() > currentSt)
        stTemp = (*StList)[currentSt];

    lastItSt = currentSt;
    ++i;

    while(i < size && currentSt < StList->size())
    {
        int tau = sigma->getItem(i);
        lastStList = StList->size();
        stTemp->getStarHelper(tau, StList, simplexes, currentSt);
        sizeList[i] += (StList->size() - lastStList);
        ++currentSt;

        if(StList->size() > currentSt)
            stTemp = (*StList)[currentSt];

        if(sizeList[i]+lastItSt == currentSt)
        {
            ++i;
            lastItSt = currentSt;
        }
    }
    if(simplexes->size() > StList->size())
    {

    }

    return simplexes;
}

vector<Simplex*>* SimplexTree::getLink(Simplex* sigma)
{
    return nullptr;
}


std::ostream& operator<<(std::ostream& os, const SimplexTree* st)
{
    return os << *st;
}

std::ostream& operator<<(std::ostream& os, const SimplexTree& st)
{

    long address = (long)&st;
    os<<"=> SIMPLEX_TREE (\naddress: "<<address<<endl;

    os<<"==> ROOT: "<<st.root <<"\n==> LEVEL: "<<st.level<<endl;
    if (st.childs == nullptr)
        os<<"==> CHILD: NULL"<<endl;
    else
        os<<"==> CHILD: "<<*(st.childs)<<endl;
    os<<"==> SIBLINGS: ";

    int size = st.getSiblingsCount();

    if (size == 0)
    {
        os << " NULL" << endl;
        return os;
    }

    for (auto i = st.siblings.begin(); i != st.siblings.end(); ++i)
    {
        os << "===> sibling: "<< *(i->second)<<endl;
    }

    return os;
}