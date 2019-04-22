//
// Created by rkindela on 07-02-19.
//

#ifndef PMTOOL_SCBUILDERTESTCASEGENERATOR_H
#define PMTOOL_SCBUILDERTESTCASEGENERATOR_H

#include "../TestCaseGenerator.h"
#include <iostream>

using namespace std;


class SCBuilderTestCaseGenerator : public TestCaseGenerator{
public:
    SCBuilderTestCaseGenerator (const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue);
    void testALL();
    void testInit();
    void executeCech();
    void executeVR();
    void executeDelaunay();
    void executeAlpha();

};


#endif //PMTOOL_SCBUILDERTESTCASEGENERATOR_H
