//
// Created by rolo on 30-08-18.
//

#ifndef PMTOOL_POLYHEDRONGENERATOR_H
#define PMTOOL_POLYHEDRONGENERATOR_H

#include "PointRepository.h"
#include "utils.h"
#include "PointDataGenerator.h"
#include "Polyhedron.h"

class PolyhedronGenerator : public PointDataGenerator
{
public:
private:


public:
    PolyhedronGenerator(int max = 100);
    Polyhedron* generatePlatonicSolid();
    Polyhedron* generateTruncatePolyhedron();
    Polyhedron* generateSphereWithRegularShapesByUse(int radius, int sides);
    Polyhedron* generateSphereWithRegularShapesByCasquete(int radius, int sides);
};


#endif //PMTOOL_POLYHEDRONGENERATOR_H
