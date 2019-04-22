//
// Created by rolo on 20-08-18.
//

#ifndef PMTOOL_SWEEPHIPERPLANE_H
#define PMTOOL_SWEEPHIPERPLANE_H


//Comparable.h - Class for comparing AVL tree nodes
// Written by Brad Appleton (1997)
// http://www.bradapp.com/ftp/src/libs/C++/AvlTrees.html

#include "Simplex.h"
#include <ostream>

using namespace std;

// ===================================================================
// Written by Glenn Burkhardt (2014)

#include <stdlib.h>

// ===================================================================
// Written by Dan Sunday (2001) (http://geomalgorithms.com/a09-_intersect-3.html)
// Modified by Glenn Burkhardt (2014) to integrate it with the AVL balanced tree
// Modified by Glenn Burkhardt (2015) to improve the sweepline segment comparison test


// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// http://geomalgorithms.com/a09-_intersect-3.html#Simple-Polygons

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdexcept>
#include "../avl.h"
#include "SimplexRepository.h"
#include "CoverSphereTree.h"
#include "../PointRepository.h"

class PointRepository;

// Assume that classes are already given for the objects:
//    Point with 2D coordinates {float x, y;}
//    Polygon with n vertices {int n; Point *V;} with V[n]=V[0]
//    Tnode is a node element structure for a BBT
//    BBT is a class for a Balanced Binary Tree
//        such as an AVL, a 2-3, or a red-black tree
//===================================================================

//===================================================================


// SweepHiperPlane sphere data struct
class SHPhsphere : public Comparable<SHPhsphere*> {
public:
    CoverSphereTree*      sphere;         // polygon edge is current half edge
    SHPhsphere* above;
    SHPhsphere* below;

    SHPhsphere() : Comparable<SHPhsphere*>(this) {}
    SHPhsphere(int center, int radious) : Comparable<SHPhsphere*>(this)
    {
        sphere = new CoverSphereTree(radious, center);
        above = below = nullptr;
    }
    ~SHPhsphere() {}

    // return true if 'this' is below 'a'
    bool operator < (const SHPhsphere& a) {

        POINT * center = PointRepository::instance()->getPoint(sphere->getCenter());
        POINT * centerA = PointRepository::instance()->getPoint(a.sphere->getCenter());

        Vector3D v(*center), u(*centerA);

        return v.squareNorm() < u.squareNorm();
    }

    bool operator== (const SHPhsphere& a) {
        return *(this->sphere) == *(a.sphere);
    }

    int squareDistance (const SHPhsphere& a) {
        return this->sphere->squareDistance(a.sphere);
    }

    cmp_t Compare(SHPhsphere* key) const {
        return (*key == *this) ? EQ_CMP
                               : ((*key < *this) ? MIN_CMP : MAX_CMP);
    }

    // debug print functions declared virtual so they're not removed as dead code
    // and are available to the debugger
    virtual ostream &
    Print(ostream & os) const {
        os << "(SHPhsphere: " << sphere << ")";
        return  os;
    }
    virtual void Print() {
        this->Print(cout) << "\n";
    }
};

    enum SPT_SIDE { SLEFT, SRIGHT, SOUT };

//===================================================================
// EventQueue Class and friends
// Event element data struct

    typedef struct _sevent SEvent;

    struct _sevent {
        CoverSphereTree*      sphere;         // sphere is a center and a radious
        enum SPT_SIDE type;    // event type: LEFT or RIGHT vertex
        int   eV;           // event vertex
        SHPhsphere* shpsphere;   // sphere in tree
    };

// the EventQueue is a presorted array (no insertions needed)

    class HSEventQueue {
        int      eventsNumber;               // total number of events in array
        int      currentEventIndex;               // index of next event on queue
        SEvent*   Edata;            // array of all events
        SEvent**  Eq;               // sorted list of event pointers
    public:
        HSEventQueue(const vector<int>& sortedSphereCenters, int radious);    // constructor
        ~HSEventQueue(void)                // destructor
        {
            delete[] Eq;
            delete[] Edata;
        }

        SEvent*   next();                    // next event on queue
    };

    typedef AvlNode<SHPhsphere*> TSnode;

// the Sweep Hiper Plane itself
    class SweepHiperPlane {
        int radious;
        SHPhsphere* shphsphere;     // current Sphere
        AvlTree<SHPhsphere*> Tree;  // balanced binary tree
    public:
        SweepHiperPlane(SHPhsphere* _shphsphere, int _radious)    // constructor
        { shphsphere = _shphsphere; radious = _radious; }
        SweepHiperPlane(int center, int _radious)    // constructor
        { shphsphere = new SHPhsphere(center, radious); radious = _radious; }
        ~SweepHiperPlane(void)               // destructor
        { cleanTree(Tree.myRoot); }

        void cleanTree(TSnode *p) {
            if (!p) return;
            delete p->Data();
            cleanTree(p->Subtree(AvlNode<SHPhsphere*>::LEFT));
            cleanTree(p->Subtree(AvlNode<SHPhsphere*>::RIGHT));
        }

        SHPhsphere*   add( SEvent* );
        SHPhsphere*   find( SEvent* );
        bool checkIntersections(SHPhsphere* );
        bool isMoveEvent(SEvent *);
        inline void moveForward(SEvent* moveEvent) {this->shphsphere = moveEvent->shpsphere;}
        void remove( SHPhsphere* );
        void removeTooFarEvents();
        void remove(TSnode * node);
    };

//===================================================================

#endif //PMTOOL_SEGMENTINTERSECTIONSCALCULATOR_H
