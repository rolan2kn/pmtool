//
// Created by rolo on 20-08-18.
//

#include "SimplePolytopeCheckingHelper.h"

SLseg* SweepLine::add( Event* E )
{
    // fill in SLseg element data
    SLseg* s = new SLseg;
    s->edge  = E->edge;
    E->seg = s;

    // if it is being added, then it must be a LEFT edge event
    // but need to determine which endpoint is the left one
    int psize = Pn->face->getVertexesCount();
    int v1 = s->edge->vertex->pointIndex;
    int eN = (s->edge->next != nullptr) ? s->edge->next->vertex->pointIndex : Pn->vertex->pointIndex;
    int v2 = eN;
    if (PointRepository::instance()->computeLexicographicOrder( v1, v2) < 0) { // determine which is leftmost
        s->leftP = v1;
        s->rightP = v2;
    }
    else {
        s->rightP = v1;
        s->leftP = v2;
    }
    s->above = (SLseg*)0;
    s->below = (SLseg*)0;

    // add a node to the balanced binary tree
    Tnode* nd = Tree.Insert(s);
    Tnode* nx = Tree.Next(nd);
    Tnode* np = Tree.Prev(nd);

    if (nx != (Tnode*)0) {
        s->above = (SLseg*)nx->Data();
        s->above->below = s;
    }
    if (np != (Tnode*)0) {
        s->below = (SLseg*)np->Data();
        s->below->above = s;
    }
    return s;
}

void SweepLine::remove( SLseg* s )
{
    // remove the node from the balanced binary tree
    Tnode* nd = Tree.Search(s);
    if (nd == (Tnode*)0) {
        const char* m = "simple_Polygon internal error:  attempt to remove segment not in tree";
        fprintf(stderr, "%s\n", m);
        POINT* lP = PointRepository::instance()->getPoint(s->leftP);
        POINT* rP = PointRepository::instance()->getPoint(s->rightP);

        fprintf(stderr, "segment: (%d, %d) --> (%d, %d)\n",
                lP->x, lP->y, rP->x, rP->y);
        Tree.DumpTree();
        throw runtime_error(m);
    }

    // get the above and below segments pointing to each other
    Tnode* nx = Tree.Next(nd);
    if (nx != (Tnode*)0) {
        SLseg* sx = (SLseg*)(nx->Data());
        sx->below = s->below;
    }
    Tnode* np = Tree.Prev(nd);
    if (np != (Tnode*)0) {
        SLseg* sp = (SLseg*)(np->Data());
        sp->above = s->above;
    }
    Tree.Delete(nd->Key());       // now can safely remove it
    delete s;  // note:  s == nd->Data()
}

// test intersect of 2 segments and return: 0=none, 1=intersect
bool SweepLine::intersect( SLseg* s1, SLseg* s2)
{
    if (s1 == (SLseg*)0 || s2 == (SLseg*)0)
        return false;      // no intersect if either segment doesn't exist

    // check for consecutive edges in polygon
    HalfEdge* e1 = s1->edge;
    HalfEdge* e2 = s2->edge;
    if ((e1->next == e2) || (e1 == e2->next))
        return false;      // no non-simple intersect since consecutive

    // test for existence of an intersect point
    double lsign, rsign;
    lsign = PointRepository::instance()->area2(s1->leftP, s1->rightP, s2->leftP);    // s2 left point sign
    rsign = PointRepository::instance()->area2(s1->leftP, s1->rightP, s2->rightP);    // s2 right point sign
    if (lsign * rsign > 0) // s2 endpoints have same sign relative to s1
        return false;      // => on same side => no intersect is possible

    lsign = PointRepository::instance()->area2(s2->leftP, s2->rightP, s1->leftP);    // s1 left point sign
    rsign = PointRepository::instance()->area2(s2->leftP, s2->rightP, s1->rightP);    // s1 right point sign
    if (lsign * rsign > 0) // s1 endpoints have same sign relative to s2
        return false;      // => on same side => no intersect is possible

    // the segments s1 and s2 straddle each other
    return true;           // => an intersect exists
}
//===================================================================

// EventQueue Routines
EventQueue::EventQueue( HalfEdge *edge )
{
    ix = 0;
    vector<int>* p = edge->face->getVertexesIndices();
    int psize = p->size();
    ne = 2 * psize;          // 2 vertex events for each edge
    Edata = (Event*)new Event[ne];
    Eq = (Event**)new Event*[ne];
    for (int i=0; i < ne; i++)          // init Eq array pointers
        Eq[i] = &Edata[i];

    // Initialize event queue with edge segment endpoints

    HalfEdge* cursor = edge;
    for (int i=0; i < psize; i++) {       // init data for edge i
        Eq[2*i]->edge = cursor;
        Eq[2*i+1]->edge = cursor;
        Eq[2*i]->eV   = (*p)[i];
        Eq[2*i]->otherEnd = Eq[2*i+1];
        Eq[2*i+1]->otherEnd = Eq[2*i];
        Eq[2*i]->seg = Eq[2*i+1]->seg = 0;

        int pi1 = (i+1 < psize) ? (*p)[i+1] : (*p)[0];
        Eq[2*i+1]->eV = pi1;
        if (PointRepository::instance()->computeLexicographicOrder( i, pi1) < 0) { // determine type
            Eq[2*i]->type   = LEFT;
            Eq[2*i+1]->type = RIGHT;
        }
        else {
            Eq[2*i]->type   = RIGHT;
            Eq[2*i+1]->type = LEFT;
        }
        cursor = cursor->next;
    }

    // Sort Eq[] by increasing x and y
    int E_compare(const void*, const void*);

    ::qsort( Eq, psize, sizeof(Event*), E_compare );
}

Event* EventQueue::next()
{
    if (ix >= ne)
        return (Event*)0;
    else
        return Eq[ix++];
}

int E_compare( const void* v1, const void* v2 ) // qsort compare two events
{
    Event**    pe1 = (Event**)v1;
    Event**    pe2 = (Event**)v2;

    int r = PointRepository::instance()->computeLexicographicOrder( (*pe1)->eV, (*pe2)->eV );
    if (r == 0) {
        if ((*pe1)->type == (*pe2)->type) return 0;
        if ((*pe1)->type == LEFT) return -1;
        else return 1;
    } else
        return r;
}

ostream & operator<<(ostream & os, SLseg* item) {
    return item->Print(os);
}

