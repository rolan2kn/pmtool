//
// Created by rkindela on 25-01-19.
//

#include "KnnClassificator.h"


KnnClassificator::KnnClassificator()
:trainingSet(), testingSet(), tagsAssingment()
{
}

KnnClassificator::~KnnClassificator()
{

}

void KnnClassificator::addTrainingPoint(int tp, const string& tag)
{
    trainingSet.push_back(tp);
    int post = trainingSet.size()-1;
    if(!tagsAssingment.insert(make_pair(post, tag)).second)
        tagsAssingment[post] = tag;
}

void KnnClassificator::addTestingPoint(int testingPoint)
{
    testingSet.push_back(testingPoint);
}

int KnnClassificator::getTrainingPoint(int pos)
{
    if(pos < 0 || pos > trainingSet.size()-1)
        return -1;

    return trainingSet[pos];
}

const string& KnnClassificator::getTagFromPoint(int pos)
{
    if(!tagsAssingment.count(pos))
        return *(new string(""));

    return tagsAssingment[pos];
}

int KnnClassificator::getTestingPoint(int pos)
{
    if(pos < 0 || pos > testingSet.size()-1)
        return -1;

    return testingSet[pos];
}

void KnnClassificator::findNeighbors(int k)
{

}