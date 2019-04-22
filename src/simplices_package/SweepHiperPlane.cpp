//
// Created by rolo on 20-08-18.
//

#include "SweepHiperPlane.h"

//===================================================================
/***
 * EventQueue::EventQueue(std::vector<int, std::allocator<int> > const&, int)'
  `SweepHiperPlane::add(_event*)'
`SweepHiperPlane::checkIntersections(SHPhsphere*)'
`SweepHiperPlane::remove(SHPhsphere*)'
 * */
// EventQueue Routines
HSEventQueue::HSEventQueue(const vector<int>& sortedSphereCenters, int radious)
{
    currentEventIndex = 0;
    int psize = sortedSphereCenters.size();

    eventsNumber = psize;          // one vertex events for each sphere

    Edata =  new SEvent[psize];
    Eq = new SEvent*[psize];

    for (int i=0; i < eventsNumber; i++)          // init Eq array pointers
        Eq[i] = &Edata[i];

    // Initialize event queue with edge segment endpoints

    for (int i=0; i < psize; i++) {       // init data for edge i
        int sphereCenter = sortedSphereCenters[i];
        Eq[i]->sphere = new CoverSphereTree(radious, sphereCenter);
        Eq[i]->eV   = sphereCenter;
        Eq[i]->shpsphere = nullptr;
    }

}

SEvent* HSEventQueue::next()
{
    if (currentEventIndex >= eventsNumber)
        return nullptr;
    else
        return Eq[currentEventIndex++];
}

//=================================================================

SHPhsphere* SweepHiperPlane::add( SEvent* E )
{
    // fill in SHPsphere element data
    SHPhsphere* s = new SHPhsphere;
    s->sphere  = E->sphere;
    E->shpsphere = s;

    // if it is being added, then it must be a LEFT edge event
    // but need to determine which endpoint is the left one

    s->above = (SHPhsphere*)0;
    s->below = (SHPhsphere*)0;

    // add a node to the balanced binary tree
    TSnode* nd = Tree.Insert(s);
    TSnode* nx = Tree.Next(nd);
    TSnode* np = Tree.Prev(nd);

    if (nx != (TSnode*)0) {
        s->above = (SHPhsphere*)nx->Data();
        s->above->below = s;
    }

    if (np != (TSnode*)0) {
        s->below = (SHPhsphere*)np->Data();
        s->below->above = s;
    }

    return s;
}

void SweepHiperPlane::remove(SHPhsphere* s)
{
    // remove the node from the balanced binary tree
    TSnode* nd = Tree.Search(s);
    if (nd == (TSnode*)0) {
        const char* m = " internal error:  attempt to remove segment not in tree";
        fprintf(stderr, "%s\n", m);

        Tree.DumpTree();
        throw runtime_error(m);
    }

    // get the above and below segments pointing to each other
    TSnode* nx = Tree.Next(nd);
    if (nx != (TSnode*)0) {
        SHPhsphere* sx = (SHPhsphere*)(nx->Data());
        sx->below = s->below;
    }
    TSnode* np = Tree.Prev(nd);
    if (np != (TSnode*)0) {
        SHPhsphere* sp = (SHPhsphere*)(np->Data());
        sp->above = s->above;
    }
    Tree.Delete(nd->Key());       // now can safely remove it
    delete s;  // note:  s == nd->Data()
}

void SweepHiperPlane::remove(TSnode* node)
{
    // remove the node from the balanced binary tree
    TSnode* nd = node;

    if (nd == (TSnode*)0) {
        const char* m = " internal error:  attempt to remove segment not in tree";
        fprintf(stderr, "%s\n", m);

        Tree.DumpTree();
        throw runtime_error(m);
    }
    SHPhsphere* s = (SHPhsphere*)node->Data();
    // get the above and below segments pointing to each other
    TSnode* nx = Tree.Next(nd);
    if (nx != (TSnode*)0) {
        SHPhsphere* sx = (SHPhsphere*)(nx->Data());
        sx->below = s->below;
    }
    TSnode* np = Tree.Prev(nd);
    if (np != (TSnode*)0) {
        SHPhsphere* sp = (SHPhsphere*)(np->Data());
        sp->above = s->above;
    }
    Tree.Delete(nd->Key());       // now can safely remove it
    delete s;  // note:  s == nd->Data()
}

SHPhsphere*   SweepHiperPlane::find( SEvent* e)
{
    SHPhsphere* s = new SHPhsphere;
    s->sphere  = e->sphere;
    Tree.Search(s);
}

bool SweepHiperPlane::checkIntersections(SHPhsphere* shS)
{
    if (shS == nullptr)
        return false;      // no intersect if either segment doesn't exist

    // check for consecutive
    SHPhsphere* below = shS->below;
    SHPhsphere* above = shS->above;

    if(shS->sphere->intersects(below->sphere))  //I need to check wich every sphere on the Tree
        shS->sphere = shS->sphere->merge(below->sphere);
    if(shS->sphere->intersects(above->sphere))
        above->sphere = shS->sphere->merge(above->sphere);
    // test for existence of an intersect point

    // the segments s1 and s2 straddle each other
    return true;           // => an intersect exists
}

bool SweepHiperPlane::isMoveEvent(SEvent * E)
{
    POINT* p0 = PointRepository::instance()->getPoint(this->shphsphere->sphere->getCenter());
    POINT* p1 = PointRepository::instance()->getPoint(E->eV);

    float distance = p0->squareDistance(*p1);
    int rad = this->radious * this->radious*4;

    return  (distance > rad);
}

void SweepHiperPlane::removeTooFarEvents()
{
    TSnode* root = Tree.myRoot;
    TSnode* prev = Tree.Prev(root);

    int rad = this->shphsphere->sphere->getRadius();
    int _rad = 0;

    while(prev != root)
    {
        SHPhsphere* s = (SHPhsphere*)prev->Data();
        int d = s->squareDistance(*this->shphsphere);

        _rad = rad + s->sphere->getRadius();

        if(d > _rad*_rad)
        {
            TSnode* temp = Tree.Prev(prev);
            this->remove(prev->Key());
            prev = temp;
        }
        else break;
    }
}

ostream & operator<<(ostream & os, SHPhsphere* item) {
    return item->Print(os);
}

