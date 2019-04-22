//
// Created by rkindela on 31-01-19.
//

#include "SimplexTreeOld.h"


SimplexTreeOld::SimplexTreeOld(int _root)
: root(_root), siblings(), childs(nullptr)
{

}

SimplexTreeOld::~SimplexTreeOld()
{
    if(childs != nullptr) // we destroy all childs first
    {
        delete childs;
    }

    int size = siblings.size();
    for (int i = 0; i < size;i++)  // then we destroy all siblings
    {
        delete siblings[i];
    }
}

void SimplexTreeOld::insert(const Simplex* new_simplex, int& pos)
{
    int simplexCard = new_simplex->getCard();
    if (pos < -1 || pos+1 > simplexCard )
        return;

   int item = new_simplex->getItem(pos);

   SimplexTreeOld* cursor = this->getSiblingByRoot(item);

   if(pos == simplexCard-1)                     // if we consume the list we are done
      return;

   if(cursor->childs == nullptr)                // if we don't have childs
   {
      item = new_simplex->getItem(pos+1);      // we get the next element
      cursor->childs = new SimplexTreeOld(item);  // and create a child with it
   }

   cursor->childs->insert(new_simplex, ++pos);      // finally we find for insection
}

SimplexTreeOld* SimplexTreeOld::getSiblingByRoot(int item)
{
    if (root == item)
        return this;

    SimplexTreeOld* cursor = this;

    int init = 0, end = this->siblings.size()-1;
    bool foundPosition = (abs(end-init) == 1);

    int middle = 0;                     // just in case we don't have siblings
    int current_root = root;

    while (!foundPosition && cursor != this)
    {
        middle = init + int((end-init)*0.5);
        current_root = this->siblings[middle]->root;

        if (current_root < item)
        {
            init = middle;
        }
        else if(current_root > item)
        {
            end = middle;
        }
        else
        {
            cursor = this->siblings[middle]; // if I found the desired root set the cursor
        }

        foundPosition = (abs(end-init) == 1);
    }

    if (foundPosition)
    {
        cursor = new SimplexTreeOld(item);

        this->siblings.insert(siblings.begin() + middle, cursor); //we insert the new element in our sorted sibling list
    }

    return cursor;
}

void SimplexTreeOld::merge(SimplexTreeOld* new_simplex_tree)
{
    if (new_simplex_tree == nullptr)
        return;

    SimplexTreeOld* cursor = this->getSiblingByRoot(new_simplex_tree->getRoot()); // we found the new root between siblings

    int siblingsCount = new_simplex_tree->getSiblingsCount();

    for(int i = 0; i < siblingsCount; i++)                                      // we get any new sibling
    {
        SimplexTreeOld* st = new_simplex_tree->getSibling(i);                      // and for each one
        SimplexTreeOld* tmp = cursor->getSiblingByRoot(st->getRoot());             // we found the sibling of the detected sibling
        tmp->merge(st);                                                                 // and merge it
    }

    SimplexTreeOld* ochild = new_simplex_tree->getChilds();                        // finally we process the son

    if (ochild == nullptr)
        return;

    if(cursor->childs == nullptr)                                               // if we don't have childs
    {
    cursor->childs = new SimplexTreeOld(ochild->getRoot());                        // we create one child with same root
    }
    cursor->childs->merge(ochild);                                              // finally we merge it both
}

std::ostream& operator<<(std::ostream& os, const SimplexTreeOld* st)
{

}

std::ostream& operator<<(std::ostream& os, const SimplexTreeOld& st)
{

}