//
// Created by rkindela on 07-02-19.
//
#include "../simplices_package/SimplicialComplexBuilder.h"
#include "../PointRepository.h"
#include "SCBuilderTestCaseGenerator.h"
#include <ctime>

SCBuilderTestCaseGenerator::SCBuilderTestCaseGenerator (const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue)
: TestCaseGenerator(file_name, trialsCount, MaxPointCount, maxValue)
{

}

void SCBuilderTestCaseGenerator::testALL()
{
    if (maxPointCount >= 214748364) // for avoid int overload
        maxPointCount = 21474836;

    string _filename = currentDirectory() + "/pmtool/Docs/" + filename;
    resultfile = ofstream(_filename);

    resultfile<<"*******************************\n * Fichero de Resultados de Pruebas SimplicialComplexesBuilder\n*******************************\n";
    resultfile<<"\n Seccion de pruebas del codigo \n";

    this->testInit();

    resultfile.flush();
    resultfile.close();
}

void SCBuilderTestCaseGenerator::testInit()
{
    PointDataGenerator pdg(maxValue);

    resultfile<<"\n ================================================================================= \n";
    resultfile <<"\nPrueba de SimplicialComplexBuilder::init en R^3 con distribucion normal, se generara un total maximo de "<<maxPointCount;
    resultfile<<"\n ================================================================================= \n";

    resultfile<<"EL maximo valor que pueden tomar los puntos tanto en x, y, z es de "<<maxValue<<"\n\n";


    resultfile<<"Se haran "<<trialsCount<<" pruebas utilizando esta distribucion variando la cantidad de puntos aleatoriamente \n\n"<<endl;


    int currentSize = trialsCount;
    int totalTrials = 0, totalPositiveSuccess = 0,totalNegativeSuccess = 0;



    for (int i = 0; i < trialsCount; i++)
    {
        int init = currentSize/trialsCount;
        int pointSize = pdg.randomInt(currentSize/trialsCount, currentSize);
        pdg.randomPointCloud(pointSize);
        std::vector<int> indexes1 = PointRepository::instance()->getIndexes(); // we get the point indexes

        cout <<endl<<"Los puntos generados son:"<<endl<<PointRepository::instance();
        int ti = (clock());
        stringstream ss;
        ss << ti;
        string _filename = currentDirectory() + "/pmtool/Docs/point_clouds/analysis_puntos_"+ ss.str() + ".txt";
        ofstream an_points(_filename);
        an_points<<"##Puntos desorganizados"<<endl;
        an_points<<PointRepository::instance();

        cout <<"\nTrial #"<<i<<": ";
        resultfile <<"\nTrial #"<<i;

        SimplicialComplexBuilder* scb = new SimplicialComplexBuilder(1);

        int size = indexes1.size();
        cout <<"\nEl arreglo original de indices: indexes 1(";
        if (size > 0)
        {
            cout <<indexes1[0];
        }
        for (int j = 1; j < size; j++)
        {
            cout << ","<<indexes1[j];
        }

        cout <<")"<<endl;


        scb->init();

        vector<int> sortedIndex = scb->getSortedPoints();

        if(size != sortedIndex.size())
        {
            cout <<"\nHubo problema el arreglo resultante tiene menos elementos que el original"<<endl;
            resultfile <<"\nHubo problema el arreglo resultante tiene menos elementos que el original"<<endl;
        }
        else
        {
            POINT p0;
            for (int j = 0; j < size; j++)
            {
                POINT* pj = PointRepository::instance()->getPoint(indexes1[j]);
                int disj = pj->squareDistance(p0);
                for (int k = 0; k < size; k++)
                {
                    POINT* pk = PointRepository::instance()->getPoint(indexes1[k]);
                    int disk = pk->squareDistance(p0);
                    if(disj >= disk)
                    {
                        int tmp = indexes1[j];
                        indexes1[j] = indexes1[k];
                        indexes1[k] = tmp;
                        disj = disk;
                    }
                }
            }

            ostringstream foo, baa;

            foo <<"indexes1 (";
            baa <<"sortedIndex (";

            if(size > 0)
            {
                foo <<indexes1[0];
                baa <<sortedIndex[0];
                cout <<endl<<"Los puntos ordenados son:"<<endl<<PointRepository::instance()->getPoint(sortedIndex[0]);
            }
            bool areEquals = true;

            for (int j = 1; j < size; j++)
            {
                foo << ","<<indexes1[j];
                baa << ","<<sortedIndex[j];

                areEquals &= indexes1[j] == sortedIndex[j];
                cout <<endl<<"Los puntos ordenados son:"<<endl<<PointRepository::instance()->getPoint(sortedIndex[j]);
            }
            an_points <<endl<<"##";
            for (int j = 0; j < size; j++)
            {
                an_points <<endl<<PointRepository::instance()->getPoint(sortedIndex[j]);
            }

            foo<<")";
            baa<<")";
            an_points.flush();
            an_points.close();
            cout<<endl<<"el resultado fue:"<<endl<<foo.str()<<endl<<baa.str()<<endl<<"son iguales: "<<areEquals<<endl;
        }

        delete scb;
        PointRepository::instance()->clear(); // remove all generated points
    }

    float pospercent = (totalPositiveSuccess*100.00)/totalTrials;
    float negpercent = (totalNegativeSuccess*100.00)/totalTrials;
    float sucpercent = ((totalPositiveSuccess+totalNegativeSuccess)*100.00)/totalTrials;

    cout <<endl<<"De un total de intentos "<<totalTrials<<", se superaron "<<totalPositiveSuccess+totalNegativeSuccess<<endl;
    cout <<"Dando un "<<sucpercent<<"% en total, las pruebas positivas superadas representan el ";
    cout <<pospercent<<" y las pruebas negativas superadas el "<<negpercent<<endl;

    resultfile<<endl<<"De un total de intentos "<<totalTrials<<", se superaron "<<totalPositiveSuccess+totalNegativeSuccess<<endl;
    resultfile<<"Dando un "<<sucpercent<<"% en total, las pruebas positivas superadas representan el ";
    resultfile<<pospercent<<" y las pruebas negativas superadas el "<<negpercent<<endl;

}

void SCBuilderTestCaseGenerator::executeCech()
{

}

void SCBuilderTestCaseGenerator::executeVR()
{

}

void SCBuilderTestCaseGenerator::executeDelaunay()
{

}

void SCBuilderTestCaseGenerator::executeAlpha()
{

}
