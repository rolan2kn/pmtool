//
// Created by rkindela on 12-04-2018.
//

#ifndef PMTOOL_EDGE_H
#define PMTOOL_EDGE_H

#include "PointRepository.h"
#include "Vector3D.h"

struct  Vertex;
struct Face;



struct HalfEdge {
    Vertex* vertex;
    HalfEdge* next;
    HalfEdge* pair;
    Face* face;

    HalfEdge();
    HalfEdge(const HalfEdge& half_edge);
    Vector3D* getOrientedVector3D();
    int getEdgeSize();
    int getDistance(HalfEdge* otherEdge);
    bool areIntersected(HalfEdge* otherEdge);
    friend std::ostream& operator<<(std::ostream& os, const HalfEdge& e);
};

struct Vertex
{
    int pointIndex; // Represent the point index in PointRepository
    HalfEdge* edge;

    friend std::ostream& operator<<(std::ostream& os, const Vertex& e);
    bool operator ==(const Vertex& e);
    vector<Face*>* getAdjacentFaces();
    vector<Vertex*>* getLink();
};

struct Face
{
    HalfEdge* edge;
    friend std::ostream& operator<<(std::ostream& os, const Face& e);
    bool operator ==(const Face& face);
    int getVertexesCount() const;
    vector<int>* getVertexesIndices();
    HalfEdge* findVertex(Vertex* v) const;
    bool inside(int pointIndex) const;
    HalfEdge* buildFromPoints(const vector<int>& pointCloud);
    bool inLeft(Face* nFace);
    bool areIntersected(Face* nFace);
    bool intersectWithSegment(int p0, int p1, int SMALL_NUM = -1);
    HalfEdge* getEdgeHoldingPoint(int pointIndex) const;
    Vector3D* getNormal();
};

#endif //PMTOOL_EDGE_H
