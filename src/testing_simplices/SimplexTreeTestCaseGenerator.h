//
// Created by rkindela on 31-01-19.
//

#ifndef PMTOOL_SIMPLEXTREETESTCASEGENERATOR_H
#define PMTOOL_SIMPLEXTREETESTCASEGENERATOR_H

#include "../TestCaseGenerator.h"
#include "../simplices_package/SimplexTree.h"

#include <iostream>


class SimplexTreeTestCaseGenerator: public TestCaseGenerator {

public:
    SimplexTreeTestCaseGenerator(const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue);
    void testALL();
    void testInsert();
    void testMerge();
    void testGetSiblingByRoot();

    void testMergeTime();
    void testInsertTime();

    void testMergeMemory();
    void testInsertMemory();

    void testGetSiblingByRootTime();

    void testGetStar();
    void testGetLink();
    void testCreateFromFile();
};


#endif //PMTOOL_SIMPLEXTREETESTCASEGENERATOR_H
