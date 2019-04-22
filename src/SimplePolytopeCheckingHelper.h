//
// Created by rolo on 20-08-18.
//

#ifndef PMTOOL_SEGMENTINTERSECTIONSCALCULATOR_H
#define PMTOOL_SEGMENTINTERSECTIONSCALCULATOR_H


//Comparable.h - Class for comparing AVL tree nodes
// Written by Brad Appleton (1997)
// http://www.bradapp.com/ftp/src/libs/C++/AvlTrees.html

#include "Polygon.h"
#include <ostream>

using namespace std;

// ===================================================================
// simple_polygon.h - Class for a polygon
// Written by Glenn Burkhardt (2014)
/*
 * simple_polygon.h
 *
 */


#include <stdlib.h>

// ===================================================================
//simple_Polygon.cpp - Check if a polygon is simple
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
#include "avl.h"
#include "PointRepository.h"
#include "HalfEdge.h"

class PointRepository;

// Assume that classes are already given for the objects:
//    Point with 2D coordinates {float x, y;}
//    Polygon with n vertices {int n; Point *V;} with V[n]=V[0]
//    Tnode is a node element structure for a BBT
//    BBT is a class for a Balanced Binary Tree
//        such as an AVL, a 2-3, or a red-black tree
//===================================================================


    enum SEG_SIDE { LEFT, RIGHT };

    class SLseg;

//===================================================================

// EventQueue Class and friends

// Event element data struct
    typedef struct _event Event;
    struct _event {
        HalfEdge*      edge;         // polygon edge i is V[i] to V[i+1]
        enum SEG_SIDE type;    // event type: LEFT or RIGHT vertex
        int   eV;           // event vertex
        SLseg* seg;   // segment in tree
        Event* otherEnd;       // segment is [this, otherEnd]
    };



// the EventQueue is a presorted array (no insertions needed)
    class EventQueue {
        int      ne;               // total number of events in array
        int      ix;               // index of next event on queue
        Event*   Edata;            // array of all events
        Event**  Eq;               // sorted list of event pointers
    public:
        EventQueue( HalfEdge *edge);    // constructor
        ~EventQueue(void)                // destructor
        { delete[] Eq;
            delete[] Edata;
        }

        Event*   next();                    // next event on queue
    };


//===================================================================


// SweepLine Class

// SweepLine segment data struct
    class SLseg : public Comparable<SLseg*> {
    public:
        HalfEdge*      edge;         // polygon edge is current half edge
        int leftP;       // leftmost vertex point
        int rightP;       // rightmost vertex point
        SLseg*   above;        // segment above this one
        SLseg*   below;        // segment below this one

        SLseg() : Comparable<SLseg*>(this) {}
        ~SLseg() {}

        // return true if 'this' is below 'a'
        bool operator< (const SLseg& a) {

            POINT * leftA = PointRepository::instance()->getPoint(leftP);
            POINT * rightA = PointRepository::instance()->getPoint(rightP);

            POINT * aleftA = PointRepository::instance()->getPoint(a.leftP);
            POINT * arightA = PointRepository::instance()->getPoint(a.rightP);
            if (leftA->x <= aleftA->x) {
                int s = PointRepository::instance()->areaSign(leftP, rightP, a.leftP);
                if (s != 0)
                    return s > 0;
                else {
                    if (leftA->x == aleftA->x)     // special case of vertical line.
                        return leftA->y < aleftA->y;
                    else
                        return PointRepository::instance()->areaSign(leftP, rightP, a.rightP) > 0;
                }
            } else {
                int s = PointRepository::instance()->areaSign(a.leftP, a.rightP, leftP);
                if (s != 0)
                    return s < 0;
                else
                    return PointRepository::instance()->areaSign(a.leftP, a.rightP, rightP) < 0;
            }
        }

        bool operator== (const SLseg& a) {
            return this->edge == a.edge;
        }

        cmp_t Compare(SLseg* key) const {
            return (*key == *this) ? EQ_CMP
                                   : ((*key < *this) ? MIN_CMP : MAX_CMP);
        }

        // debug print functions declared virtual so they're not removed as dead code
        // and are available to the debugger
        virtual ostream &
        Print(ostream & os) const {
            POINT* lP = PointRepository::instance()->getPoint(leftP);
            POINT* rP = PointRepository::instance()->getPoint(rightP);
            os << "(" << lP->x << ", " << lP->y << "; " << rP->x << ", " << rP->y << ")";
            return  os;
        }
        virtual void Print() {
            this->Print(cout) << "\n";
        }
    };



    typedef AvlNode<SLseg*> Tnode;

// the Sweep Line itself
    class SweepLine {
        int      nv;           // number of vertices in polygon
        HalfEdge* Pn;     // initial Edge
        AvlTree<SLseg*> Tree;  // balanced binary tree
    public:
        SweepLine(HalfEdge *P)    // constructor
        { nv = P->face->getVertexesCount(); Pn = P; }
        ~SweepLine(void)               // destructor
        { cleanTree(Tree.myRoot); }

        void cleanTree(Tnode *p) {
            if (!p) return;
            delete p->Data();
            cleanTree(p->Subtree(AvlNode<SLseg*>::LEFT));
            cleanTree(p->Subtree(AvlNode<SLseg*>::RIGHT));
        }

        SLseg*   add( Event* );
        SLseg*   find( Event* );
        bool     intersect( SLseg*, SLseg* );
        void     remove( SLseg* );
    };

//===================================================================

#endif //PMTOOL_SEGMENTINTERSECTIONSCALCULATOR_H
