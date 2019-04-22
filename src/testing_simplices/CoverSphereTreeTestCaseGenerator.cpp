//
// Created by rkindela on 25-01-19.
//

#include "CoverSphereTreeTestCaseGenerator.h"
#include "../simplices_package/CoverSphereTree.h"


CoverSphereTreeTestCaseGenerator::CoverSphereTreeTestCaseGenerator(const std::string& file_name, int trialsCount, int MaxPointCount, int maxValue)
:TestCaseGenerator(file_name, trialsCount, MaxPointCount, maxValue)
{

}

void CoverSphereTreeTestCaseGenerator::testALL()
{
    if (maxPointCount >= 214748364) // for avoid int overload
        maxPointCount = 21474836;

    string _filename = currentDirectory() + "/pmtool/Docs/" + filename;
    resultfile = ofstream(_filename);

    resultfile<<"*******************************\n * Fichero de Resultados de Pruebas \n*******************************\n";
    resultfile<<"\n Seccion de pruebas del codigo \n";

   this->testHasInside();
   this->testIntersects();
   this->testMerge();
    resultfile.flush();
    resultfile.close();
}

void CoverSphereTreeTestCaseGenerator::testHasInside()
{
    PointDataGenerator pdg(maxValue);

    resultfile<<"\n ================================================================================= \n";
    resultfile <<"\nPrueba de CoverSphereTree::hasInside en R^3 con distribucion normal, se generara un total maximo de "<<maxPointCount;
    resultfile<<"\n ================================================================================= \n";

    resultfile<<"EL maximo valor que pueden tomar los puntos tanto en x, y, z es de "<<maxValue<<"\n\n";

    resultfile<<"Se haran "<<trialsCount<<" pruebas utilizando esta distribucion variando la cantidad de puntos aleatoriamente \n\n"<<endl;


    int currentSize = trialsCount;
    int totalTrials = 0, totalPositiveSuccess = 0,totalNegativeSuccess = 0 ;

    vector<CoverSphereTree*> cstList;
    for (int i = 0; i < trialsCount; i++)
    {
        int init = currentSize/trialsCount;
        int pointSize = pdg.randomInt(currentSize/trialsCount, currentSize);
        pdg.randomPointCloud(pointSize);
        std::vector<int> indexes1 = PointRepository::instance()->getIndexes(); // we get the point indexes

        cout <<"\nTrial #"<<i<<": ";
        resultfile <<"\nTrial #"<<i;

        int size = indexes1.size();
        for (int j = 0; j < size;j++)
        {
            int radious = pdg.randomInt(1, maxValue*2);
            cstList.push_back(new CoverSphereTree(radious, indexes1[j]));
        }

        for(int k = 0; k < size; k++)
        {
            for(int l = 0; l < size; l++)
            {
                if (l == k) continue;

                ++totalTrials;

                int rk = cstList[k]->getRadius();
                int rl = cstList[l]->getRadius();

                int ck = cstList[k]->getCenter();
                int cl = cstList[l]->getCenter();

                int dif = PointRepository::instance()->squareMagnitude(ck, cl);
                int radious_dif =  abs(rk-rl)*abs(rk-rl);
                bool expected_result = (dif <= radious_dif);

                if(cstList[k]->hasInside(cstList[l]) && expected_result)
                {
                    ++totalPositiveSuccess;
                    cout <<endl<<"PRUEBA POSITIVA SUPERADA";
                }
                else if(!cstList[k]->hasInside(cstList[l]) && !expected_result)
                    {
                        ++totalNegativeSuccess;
                        cout <<endl<<"PRUEBA NEGATIVA SUPERADA";
                    }
                else
                    {
                        cout <<endl<<"PRUEBA FALLIDA";
                    }
            }
        }

        for (int k = 0; k < size; k++)
        {
            if (cstList[k] != nullptr)
                delete cstList[k];
        }
        cstList.clear();

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

    resultfile.flush();
}


void CoverSphereTreeTestCaseGenerator::testIntersects()
{
    PointDataGenerator pdg(maxValue);

    resultfile<<"\n ================================================================================= \n";
    resultfile <<"\nPrueba de CoverSphereTree::intersects en R^3 con distribucion normal, se generara un total maximo de "<<maxPointCount;
    resultfile<<"\n ================================================================================= \n";

    resultfile<<"EL maximo valor que pueden tomar los puntos tanto en x, y, z es de "<<maxValue<<"\n\n";

    resultfile<<"Se haran "<<trialsCount<<" pruebas utilizando esta distribucion variando la cantidad de puntos aleatoriamente \n\n"<<endl;


    int currentSize = trialsCount;
    int totalTrials = 0, totalPositiveSuccess = 0, totalNegativeSuccess = 0;

    vector<CoverSphereTree*> cstList;
    for (int i = 0; i < trialsCount; i++)
    {
        int init = currentSize/trialsCount;
        int pointSize = pdg.randomInt(currentSize/trialsCount, currentSize);
        pdg.randomPointCloud(pointSize);   // generate pointCount points in normal distribution and stored all points in PointRepository

        std::vector<int> indexes1 = PointRepository::instance()->getIndexes(); // we get the point indexes

        cout <<"\nTrial #"<<i<<": ";
        resultfile <<"\nTrial #"<<i;

        int size = indexes1.size();
        for (int j = 0; j < size;j++)
        {
            int radious = pdg.randomInt(1, maxValue*2);
            cstList.push_back(new CoverSphereTree(radious, indexes1[j]));
        }

        for(int k = 0; k < size; k++)
        {
            for(int l = 0; l < size; l++)
            {
                if (l == k) continue;

                int rk = cstList[k]->getRadius();
                int rl = cstList[l]->getRadius();

                int ck = cstList[k]->getCenter();
                int cl = cstList[l]->getCenter();


                int dif = PointRepository::instance()->squareMagnitude(ck, cl);
                int radious_sum =  abs(rk+rl)*abs(rk+rl);
                bool expected_result = (dif <= radious_sum);

                ++totalTrials;

                if(cstList[k]->intersects(cstList[l]) && expected_result)
                {
                    cout <<endl<<"PRUEBA POSITIVA SUPERADA";
                    ++totalPositiveSuccess;
                }
                else if(!cstList[k]->hasInside(cstList[l]) && !expected_result)
                {
                    cout <<endl<<"PRUEBA NEGATIVA SUPERADA";
                    ++totalNegativeSuccess;
                }
                else
                {
                    cout <<endl<<"PRUEBA FALLIDA";
                };
            }
        }

        for (int k = 0; k < size; k++)
        {
            if (cstList[k] != nullptr)
                delete cstList[k];
        }
        cstList.clear();

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


    resultfile.flush();
}

void CoverSphereTreeTestCaseGenerator::testMerge()
{
    PointDataGenerator pdg(maxValue);

    resultfile<<"\n ================================================================================= \n";
    resultfile <<"\nPrueba de CoverSphereTree::merge en R^3 con distribucion normal, se generara un total maximo de "<<maxPointCount;
    resultfile<<"\n ================================================================================= \n";

    resultfile<<"EL maximo valor que pueden tomar los puntos tanto en x, y, z es de "<<maxValue<<"\n\n";

    resultfile<<"Se haran "<<trialsCount<<" pruebas utilizando esta distribucion variando la cantidad de puntos aleatoriamente \n\n"<<endl;


    int currentSize = trialsCount;

    vector<CoverSphereTree*> cstList;
    vector<CoverSphereTree*> mergeCSTList;


    int totalTrials = 0, totalSuccess = 0;
    for (int i = 0; i < trialsCount; i++)
    {
        int init = currentSize/trialsCount;
        int pointSize = pdg.randomInt(currentSize/trialsCount, currentSize);
        pdg.randomPointCloud(pointSize);   // generate pointCount points in normal distribution and stored all points in PointRepository

        std::vector<int> indexes1 = PointRepository::instance()->getIndexes(); // we get the point indexes

        cout <<"\nTrial #"<<i<<": ";
        resultfile <<"\nTrial #"<<i;


        int size = indexes1.size();
        for (int j = 0; j < size;j++)
        {
            int radious = pdg.randomInt(1, maxValue*2);
            cstList.push_back(new CoverSphereTree(radious, indexes1[j]));
        }

        for(int k = 0; k < size; k++)
        {
            int dimension = pdg.randomInt(1, size-1), count = 0;

            CoverSphereTree* cstMerge = new CoverSphereTree(*cstList[k]);

            for(int l = 0; l < size; l++)
            {
                if (l == k) continue;

                if(count == dimension)
                    break;
                ++count;

                ++totalTrials;

                int rk = cstMerge->getRadius();
                int rl = cstList[l]->getRadius();

                int cl = cstList[l]->getCenter();
                int ck = cstMerge->getCenter();

                int dif = PointRepository::instance()->squareMagnitude(ck, cl);
                POINT P0 = *PointRepository::instance()->getPoint(ck);
                POINT P1 = *PointRepository::instance()->getPoint(cl);

                POINT pc = (P0 + P1).multiply(0.5); //center point
                int radious = int((rk+rl+int(sqrt(dif)))*0.5);

                cout <<endl<<"############### fusion k = "<<k<<", l= "<<l<<":";
                cstMerge = cstMerge->merge(cstList[l]);

                POINT* c = PointRepository::instance()->getPoint(cstMerge->getCenter());

                if(*c == pc && radious == cstMerge->getRadius())
                {
                    cout<<endl<<"Prueba SUPERADA";
                    ++totalSuccess;
                }
                else
                    cout<<endl<<"Prueba FALLIDA";
                cout.flush();
            }
            mergeCSTList.push_back(cstMerge);
        }

        int csize = mergeCSTList.size();
        for (int k = csize-1; k > 0; k--)
        {
            if (mergeCSTList[k] != nullptr && mergeCSTList[k]->getCenter() > size-1)
            {
                mergeCSTList[k]->clearChilds();
                delete mergeCSTList[k];
                cout <<"\nEliminado merge "<<k;
            }
        }
        cout << "\nInicio de borrado de mergelist";
        mergeCSTList.clear();
        cout << "\nBorrando mergelist";
        for (int k = 0; k < size; k++)
        {
            if (cstList[k] != nullptr) {
                delete cstList[k];
                cout << "\nEliminado cstList element "<<k;
            }
        }
        cout << "\nIntento de borrar cstList";
        cstList.clear();
        cout << "\nSe borro cstList";
        vector<CoverSphereTree*> cstList;
        vector<CoverSphereTree*> mergeCSTList;


        cout << "\nintento de borrado de PointRepository";
        PointRepository::instance()->clear(); // remove all generated points
        cout << "\nse borro PointRepository";
    }

    float percentSuccess = (totalSuccess*100.00f)/totalTrials;
    cout <<endl<<"De un total de intentos "<<totalTrials<<", se superaron "<<totalSuccess<<" dando un "<<percentSuccess<<"%"<<endl;
    resultfile <<endl<<"De un total de intentos "<<totalTrials<<", se superaron "<<totalSuccess<<" dando un "<<percentSuccess<<"%"<<endl;

    resultfile.flush();
}

void CoverSphereTreeTestCaseGenerator::testBuildSimplexTree()
{

}

CoverSphereTreeTestCaseGenerator::~CoverSphereTreeTestCaseGenerator()
{

}