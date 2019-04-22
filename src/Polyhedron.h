//
// Created by rkindela on 19-07-18.
//

#ifndef PMTOOL_POLYHEDRON_H
#define PMTOOL_POLYHEDRON_H

# include "Polygon.h"
#include <vector>
#include "HalfEdge.h"
#include "BoundingSphere.h"
#include <map>
#include <stack>

using namespace std;

class Polyhedron {
private:
    vector<Face*> faces;
    vector<Vertex*> vertexes;
    vector<HalfEdge*> edges;
public:
    Polyhedron();
    vector<int>* getLink(int point);

    void buildFromPoints(std::vector<int> pointcloud);
    int addVertex(int pointIndex);
    int addFace();
    int addEdge(int idxVertex, int idxFace);

    POINT* getPoint(int pos);
    POINT* getPoint(int pos) const;

    int volume(int faceIdx, int pointIdx);

    Face* getFace(int pos);
    Face* getFace(int pos) const;

    Vertex* getVertex(int pos);
    Vertex* getVertex(int pos) const;

    HalfEdge* getEdge(int pos);
    HalfEdge* getEdge(int pos) const;

    int getPointCount() const;
    int getFaceCount() const;
    int getEdgeCount() const;

    bool intersect(const Polyhedron& newPoly);

    bool isRegular();
    bool isSimple();
    bool isConvex();
    bool inside(int npoint);

    bool contains(const Polyhedron& p) const;
    int pointIntersections(int point);
    BoundingSphereTree* getBoundingSphere() const;
    bool left(int npoint);
    bool rigth(int npoint);
    float area2();
    std::vector<Polyhedron*> triangulate();

    Polyhedron* getDualPolyhedron();
    vector<Face*>* getConvexHull();
    vector<Face*>* giftWrapping();
    vector<Face*>* grahamScan();
    vector<Face*>* incremental();

    friend std::ostream& operator<<(std::ostream& os, const Polyhedron* poly);
    friend std::ostream& operator<<(std::ostream& os, const Polyhedron& poly);
};

#endif //PMTOOL_POLYHEDRON_H
