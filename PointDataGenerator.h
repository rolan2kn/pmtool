//
// Created by rkindela on 27-05-18.
//

#ifndef PMTOOL_POINTDATAGENERATOR_H
#define PMTOOL_POINTDATAGENERATOR_H

#include "PointRepository.h"
#include "utils.h"

class PointDataGenerator {

protected:
    std::vector<POINT*> generateOnHullPoints(int radius, int hullSize, int dimension = 2);
    std::vector<POINT*> generateInsideHullPoints(int radius, int insideAmount);
    int maximum;
public:
    PointDataGenerator(int max = 100):maximum(max){};
    int randomInt(int min, int max);
    POINT* randomPoint(int min, int max);
    void randomPointCloud(int pointCount);
    void randomPointCloudWithPercentInside(int pointCount, float percent);
    void randomPointCloudWithPercentOnHull(int pointCount, float percent);
    void randomPointCloudWithPercentOn3DHull(int pointCount, float percent);
    vector<int>* getRandomPermutation(const vector<int>& pointCloud, int exceptionPoint);
};


#endif //PMTOOL_POINTDATAGENERATOR_H
