//
// Created by rkindela on 09-02-19.
//

#ifndef PMTOOL_SWEEPHIPERPLANETESTCASEGENERATOR_H
#define PMTOOL_SWEEPHIPERPLANETESTCASEGENERATOR_H


#include "../TestCaseGenerator.h"
#include "../simplices_package/SimplexTree.h"

#include <iostream>


class SweepHiperPlaneTestCaseGenerator: public TestCaseGenerator {

public:
    SweepHiperPlaneTestCaseGenerator(const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue);
    void testALL();
    void   testAddSHPSpherea();
    void  testFind();
    void testCheckIntersections();
    void testIsMoveEvent();
    void testRemove();
    void testRemoveTooFarEvents();
};

#endif //PMTOOL_SWEEPHIPERPLANETESTCASEGENERATOR_H
