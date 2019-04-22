//
// Created by rkindela on 25-01-19.
//

#ifndef PMTOOL_KNNCLASSIFICATOR_H
#define PMTOOL_KNNCLASSIFICATOR_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class KnnClassificator {
    private:
        vector<int> trainingSet;
        vector<int> testingSet;
        map <int, string> tagsAssingment;
    public:
        KnnClassificator();
        ~KnnClassificator();
        void addTrainingPoint(int tp, const string& tag);
        void addTestingPoint(int testingPoint);
        int getTrainingPoint(int pos);
        const string& getTagFromPoint(int pos);
        int getTestingPoint(int pos);
        void findNeighbors(int k);
};


#endif //PMTOOL_KNNCLASSIFICATOR_H
