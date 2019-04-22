//
// Created by rkindela on 25-01-19.
//

#ifndef PMTOOL_CSTTESTCASEGENERATOR_H
#define PMTOOL_CSTTESTCASEGENERATOR_H

#include "../TestCaseGenerator.h"
#include <iostream>

using namespace std;


class CoverSphereTreeTestCaseGenerator : public TestCaseGenerator {

private:

public:
    CoverSphereTreeTestCaseGenerator(const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue);
    void testALL();

    void testHasInside();
    void testIntersects();
    void testMerge();
    void testBuildSimplexTree();

    ~CoverSphereTreeTestCaseGenerator();
};


#endif //PMTOOL_CSTTESTCASEGENERATOR_H
