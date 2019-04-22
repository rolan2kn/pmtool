//
// Created by rkindela on 31-01-19.
//

#include "SimplexTreeTestCaseGenerator.h"
#include "../simplices_package/SimplexRepository.h"

SimplexTreeTestCaseGenerator::SimplexTreeTestCaseGenerator(const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue)
:TestCaseGenerator(file_name, trialsCount, MaxPointCount, maxValue)
{

}

void SimplexTreeTestCaseGenerator::testALL()
{
    if (maxPointCount >= 214748364) // for avoid int overload
        maxPointCount = 21474836;

    string _filename = currentDirectory() + "/pmtool/Docs/" + filename;
    resultfile = ofstream(_filename);

    resultfile<<"*******************************\n * Fichero de Resultados de Pruebas de SIMPLEX TREE \n*******************************\n";
    resultfile<<"\n Seccion de pruebas del codigo \n";

//    testInsert();
//    testMerge();
//    testGetSiblingByRoot();
//
//    testMergeTime();
//    testInsertTime();
//
//    testMergeMemory();
//    testInsertMemory();
//
//    testGetSiblingByRootTime();
    testCreateFromFile();

    resultfile.flush();
    resultfile.close();
}

void SimplexTreeTestCaseGenerator::testInsert()
{
    PointDataGenerator pdg(maxValue);

    resultfile<<"\n ================================================================================= \n";
    resultfile <<"\nPrueba de SimplexTree::Insert en R^3 con distribucion normal, se generara un total maximo de "<<maxPointCount;
    resultfile<<"\n ================================================================================= \n";

    resultfile<<"El maximo valor que pueden tomar los puntos tanto en x, y, z es de "<<maxValue<<"\n\n";

    resultfile<<"Se haran "<<trialsCount<<" pruebas utilizando esta distribucion variando la cantidad de puntos aleatoriamente \n\n"<<endl;

    int currentSize = trialsCount;

    for (int i = 0; i < trialsCount; i++)
    {
        int pointSize = pdg.randomInt(currentSize/trialsCount, currentSize);
        pdg.randomPointCloud(pointSize);
        std::vector<int> indexes1 = PointRepository::instance()->getIndexes(); // we get the point indexes

        cout <<"\nTrial #"<<i<<": ";
        resultfile <<"\nTrial #"<<i;

        int size = indexes1.size();
        int last = SimplexRepository::instance()->getSimplexCount();

        for(int j = 0; j < size; ++j) /// we generate K0 simplicial complexes
        {
            Simplex* s = new Simplex();
            s->addItem(indexes1[j]);
            s->setId(j+last);
            SimplexRepository::instance()->addSimplex(s);
        }

        last = SimplexRepository::instance()->getSimplexCount();
        SimplexRepository::instance()->setKSimplexCountByDimension(0, size);
        SimplexTree* simplexTree = new SimplexTree(-1);

        int k1Count = pdg.randomInt(1, size-1);
        for(int k = 0; k < k1Count; ++k) /// we generate K1 simplicial complexes
        {
            int k0 = pdg.randomInt(0, size);
            int elem = k0;
            while(elem == k0)
            {
                elem = pdg.randomInt(0, size);
            }
            Simplex* k1 = new Simplex();
            k1->addItem(k0);
            k1->addItem(elem);

            k1->setId(k+last);

            SimplexRepository::instance()->addSimplex(k1);
        }
        SimplexRepository::instance()->setKSimplexCountByDimension(1, k1Count);

        last = SimplexRepository::instance()->getSimplexCount();
        int k2Count = pdg.randomInt(1, size-2);
        for(int k = 0; k < k2Count; ++k) /// we generate K2 simplicial complexes
        {
            int init = SimplexRepository::instance()->getKSimplexCountByDimension(0);
            int k0 = pdg.randomInt(init, last);
            int k1 = k0, k2;
            while(k1 == k0)
            {
                k1 = pdg.randomInt(init, last);
            }
            k2 = k1;
            while(k2 == k1 || k2 == k0)
            {
                k2 = pdg.randomInt(init, last);
            }

            Simplex* k2_s = new Simplex();
            k2_s->addItem(k0);
            k2_s->addItem(k1);
            k2_s->addItem(k2);
            k2_s->setId(k+last);

            SimplexRepository::instance()->addSimplex(k2_s);
        }

        last = SimplexRepository::instance()->getSimplexCount();
        SimplexRepository::instance()->setKSimplexCountByDimension(2, k2Count);
        int k3Count = pdg.randomInt(1, size-3);
        for(int k = 0; k < k3Count; ++k) /// we generate K3 simplicial complexes
        {
            int init = SimplexRepository::instance()->getKSimplexCountByDimension(2);
            int k0 = pdg.randomInt(init, last);
            int k1 = k0, k2, k3;
            while(k1 == k0)
            {
                k1 = pdg.randomInt(init, last);
            }
            k2 = k1;
            while(k2 == k1 || k2 == k0)
            {
                k2 = pdg.randomInt(init, last);
            }
            k3 = k2;
            while(k3 == k2 || k3 == k1 || k3 == k0)
            {
                k3 = pdg.randomInt(init, last);
            }

            Simplex* k3_s = new Simplex();
            k3_s->addItem(k0);
            k3_s->addItem(k1);
            k3_s->addItem(k2);
            k3_s->addItem(k3);
            k3_s->setId(k+last);

            SimplexRepository::instance()->addSimplex(k3_s);
        }

        int total = SimplexRepository::instance()->getSimplexCount();
        for(int t = 0;t < total;t++ )
        {
            Simplex* s = SimplexRepository::instance()->getSimplex(t);
            simplexTree->insert(s, 0);
            cout <<"Se inserto el simplex: "<<t<<"\n"<<s;
            resultfile <<"Se inserto el simplex: "<<t<<"\n"<<s;
        }

        cout <<"\n Se intenta borrar el simplex tree";
        delete simplexTree;
        cout <<"\n Se borra el simplex tree";

        PointRepository::instance()->clear(); // remove all generated points
        SimplexRepository::instance()->clear();
    }

    resultfile.flush();
}

void SimplexTreeTestCaseGenerator::testMerge()
{

}

void SimplexTreeTestCaseGenerator::testGetSiblingByRoot()
{

}

void SimplexTreeTestCaseGenerator::testMergeTime()
{

}

void SimplexTreeTestCaseGenerator::testInsertTime()
{

}

void SimplexTreeTestCaseGenerator::testMergeMemory()
{

}

void SimplexTreeTestCaseGenerator::testInsertMemory()
{

}

void SimplexTreeTestCaseGenerator::testGetSiblingByRootTime()
{

}

void SimplexTreeTestCaseGenerator::testGetStar()
{

}

void SimplexTreeTestCaseGenerator::testGetLink()
{

}

void SimplexTreeTestCaseGenerator::testCreateFromFile()
{
    string _filename = currentDirectory() + "/pmtool/Docs/SIMPLEX_TREES/simplex_tree_19.02.27__00.25.04.txt";
    SimplexTree * smlpxTree = new SimplexTree(_filename);

    Simplex* s = new Simplex();
    s->addItem(101);
    vector<Simplex*>* star = smlpxTree->getStar(s);

    for(auto tau: (*star))
    {
        cout<<tau;
    }
}