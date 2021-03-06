//
// Created by rkindela on 11-06-18.
//

#ifndef PMTOOL_TESTCASEGENERATOR_H
#define PMTOOL_TESTCASEGENERATOR_H

#include "PointDataGenerator.h"
#include "Polygon.h"
#include "Vector3D.h"
#include <fstream>
#include <sstream>


class TestCaseGenerator {
private:
    int trialsCount;
    int maxPointCount;
    int maxValue;
    std::ofstream resultfile;
    std::string filename;
protected:
public:
    void helper4SingletonChecking();
    void helper4Area2Testing(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2);
    void helper4LowerXandYTesting(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2);
    void helper4SortedPolarAngleTesting(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2);
    void helper4RandomIntTesting();
    void helper4RandomPointTesting();
    void helper4RandomPointCloudTesting();
public:
    TestCaseGenerator(const std::string& file_name, int trialsCount, int MaxPointCount);
    void testALL();
    void testSimpleRandomDistribution();
    void testPolygon();
    void testInsidePointDistribution();
    void testInHullPointDistribution();
    void testPoint();
    void testVector();
    void testPointRepository();
    void testPointDataGenerator();
    ~TestCaseGenerator();
};


#endif //PMTOOL_TESTCASEGENERATOR_H
