//
// Created by rolo on 21-01-19.
//

#include "SimplicialComplexBuilder.h"
#include "../PointRepository.h"



/***
 * SimplicialComplexBuilder.
 *
 * **** Purpose:
 *
 * Let S to be a point cloud. The purpose of this class is to construct a Simplicial Complex K using a valor r.
 * To do that, you can select your favorite approach using a
 * To build a specific level r from a filtration f, where the level is desired to be equal to the radious r.
 * But if all points in cloud is too disperses the we need transtitions from values ri to rj as big enough
 * to produce changes in topology, so if j > i is not mandatory that rj - ri = 1, so transitions between
 * simplical complexes Ki to Kj can be produced by radious changes superiors to 1.
 * This allow us to avoid memory overhead.
 */



SimplicialComplexBuilder::SimplicialComplexBuilder(int radious)
: sortedPoints(), currentRadious(radious), hiperSphereEventQueue(nullptr), sweepHiperPlane(nullptr)
{
    this->init();
}

void SimplicialComplexBuilder::init()
{
    vector<int> indexes = PointRepository::instance()->getIndexes();
    int lowestXcoord = PointRepository::instance()->getLowerMostXCoord(indexes);

    if (lowestXcoord < 0 || indexes.size() < 3)
        return ;

    int temp = indexes[lowestXcoord];              // we swap the 0 point with the lowest X
    indexes[lowestXcoord] = indexes[0];
    indexes[0] = temp;

    sortedPoints = PointRepository::instance()->getSortedPoints(indexes); // we sort all points using its angle


    sweepHiperPlane = new SweepHiperPlane(sortedPoints[0], currentRadious);
}


Simplex* SimplicialComplexBuilder::executeCech(Filtration* f, int radious)
{
    // simple_Polygon(): test if a Polygon P is simple or not
    //     Input:
    //     Return: false = is NOT simple
    //             true  = IS simple

    hiperSphereEventQueue = new HSEventQueue(sortedPoints, currentRadious);
    SEvent*      currentEvent = nullptr;                 // the current event
    SHPhsphere* s = nullptr;

    // This loop processes all events in the sorted queue
    // Events are only left or right vertices since
    // No new events will be added (an intersect => Done)

    while ((currentEvent = hiperSphereEventQueue->next())) {      // while there are events

        if (sweepHiperPlane->isMoveEvent(currentEvent)) {     // process a left vertex

            sweepHiperPlane->checkIntersections(currentEvent->shpsphere);
            sweepHiperPlane->moveForward(currentEvent);

            sweepHiperPlane->removeTooFarEvents();     // remove it from the sweep line


//            s = sweepHiperPlane->add(currentEvent);         // add it to the sweep line
//            if (sweepHiperPlane->checkIntersections( s ))
//                return nullptr;          // Pn is NOT simple
        }
        else
        {
            sweepHiperPlane->add(currentEvent);
            sweepHiperPlane->checkIntersections(currentEvent->shpsphere);

        }
    }
    return nullptr;      // Pn is simple
}

Simplex* SimplicialComplexBuilder::executeVR(Filtration* f, int radious)
{

}

Simplex* SimplicialComplexBuilder::executeDelaunay(Filtration* f, int radious)
{

}

Simplex* SimplicialComplexBuilder::executeAlpha(Filtration* f, int radious)
{

}
