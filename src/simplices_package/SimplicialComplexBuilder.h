//
// Created by rolo on 21-01-19.
//

#ifndef PMTOOL_SIMPLICIALCOMPLEXBUILDER_H
#define PMTOOL_SIMPLICIALCOMPLEXBUILDER_H

#include <vector>
using namespace std;

#include "Simplex.h"
#include "Filtration.h"
#include "SimplexRepository.h"
#include "SweepHiperPlane.h"

class SimplicialComplexBuilder
{
private:
    vector<int> sortedPoints;
    HSEventQueue*  hiperSphereEventQueue;
    SweepHiperPlane* sweepHiperPlane;
    int currentRadious;
public:
    SimplicialComplexBuilder(int initialRadious);
    void init();
    Simplex* executeCech(Filtration* f, int radious);
    Simplex* executeVR(Filtration* f, int radious);
    Simplex* executeDelaunay(Filtration* f, int radious);
    Simplex* executeAlpha(Filtration* f, int radious);

    const vector<int>& getSortedPoints() {return sortedPoints;};
};


#endif //PMTOOL_SIMPLICIALCOMPLEXBUILDER_H
