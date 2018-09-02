//
// Created by rkindela on 12-04-2018.
//

#include "HalfEdge.h"
#include "Vector3D.h"


std::ostream& operator<<(std::ostream& os, const HalfEdge& e) {
    os << "HALF_EDGE address: " << long(&e) << endl;
    if (e.vertex != nullptr)
        os << "HalfEdge: { " << e.vertex << endl;
    else
        os << "HalfEdge:{ vertex: nullptr" << endl;

    /**
     * Vertex* vertex;
    HalfEdge* next;
    HalfEdge* pair;
     Face* face;
     */

    if (e.next != nullptr)
        os << ", NEXT: " << (*e.next) << endl;
    else
        os << ", NEXT: nullptr" << endl;

    if (e.pair != nullptr)
        os << ", PAIR: " << (*e.pair) << endl;
    else
        os << ", PAIR: nullptr" << endl;

    if (e.face != nullptr)
        os << ", " << (*e.face) << endl;
    else
        os << ", FACE: nullptr" << endl;

    os << "}\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vertex& e)
{
    POINT* point = PointRepository::instance()->getPoint(e.pointIndex);
    if (point != nullptr)
        os<<" VERTEX: { "<<point;
    else
        os << "VERTEX: POINT: nullptr";

    if (e.edge != nullptr)
        os<<", EDGE: "<<(*e.edge);
    else
        os<<", EDGE: nullptr";
    os <<"}\n";

    return os;
}

std::ostream& operator<<(std::ostream& os, const Face& e)
{
    if (e.edge != nullptr)
        os<<"FACE: { EDGE: "<<(*e.edge);
    else
        os<<"FACE: { EDGE: nullptr";
    os <<"}\n";

    return os;
}

HalfEdge::HalfEdge(): vertex(nullptr), next(nullptr), pair(nullptr), face(nullptr)
{

}

HalfEdge::HalfEdge(const HalfEdge& half_edge): vertex(half_edge.vertex), next(half_edge.next), pair(half_edge.pair), face(half_edge.face)
{

}

int HalfEdge::getDistance(HalfEdge* otherEdge)
{
    HalfEdge* cursor = this;
    int count = 0;

    if (otherEdge == nullptr)
        return -1;

    if(otherEdge == cursor)
        return 0;

    do{
        ++count;
        cursor = cursor->next;
    }while(cursor != this && cursor != otherEdge);

    if(cursor == otherEdge)
        return count;

    return -1;
}

int HalfEdge::getEdgeSize()
{
    int pa = -1;
    int pb = -1;
    if (next != nullptr && next->vertex != nullptr)
        pb = next->vertex->pointIndex;
    if (vertex != nullptr)
        pa = vertex->pointIndex;
    return PointRepository::instance()->squareMagnitude(pa, pb);
}

Vector3D* HalfEdge::getOrientedVector3D()
{
    int pa = -1;
    int pb = -1;
    if (next != nullptr && next->vertex != nullptr)
        pb = next->vertex->pointIndex;
    if (vertex != nullptr)
        pa = vertex->pointIndex;

    return PointRepository::instance()->getVector3dFromPoints(pa, pb);
}

bool HalfEdge::areIntersected(HalfEdge* otherEdge)
{
    if(otherEdge == nullptr ||
       otherEdge->next == nullptr ||
       this->next == nullptr)
        return false;
    if(otherEdge->vertex == nullptr ||
       otherEdge->next->vertex == nullptr ||
        this->vertex == nullptr ||
        this->next->vertex == nullptr)
        return false;

    int p1 = this->vertex->pointIndex;
    int q1 = this->next->vertex->pointIndex;
    int p2 = otherEdge->vertex->pointIndex;
    int q2 = otherEdge->next->vertex->pointIndex;

    return PointRepository::instance()->areIntersected(p1, q1, p2, q2);

}

bool Vertex::operator ==(const Vertex& e)
{
    return (pointIndex == e.pointIndex);
}

vector<Face*>* Vertex::getAdjacentFaces()
{
    vector<Face*>* adjacentFaces = new vector<Face*>();

    if(edge == nullptr)
        return adjacentFaces;

    HalfEdge* cursor = edge;
    do
        {
            adjacentFaces->push_back(cursor->face);
            if (cursor->pair != nullptr)
            cursor = cursor->pair->next;
        }while(cursor != edge && cursor != nullptr);

    return adjacentFaces;
}

vector<Vertex*>* Vertex::getLink()
{
    vector<Vertex*>* oppositeVertexes = new vector<Vertex*>();

    if(edge == nullptr)
        return oppositeVertexes;

    HalfEdge* cursor = edge;
    do
    {
        HalfEdge* temp = cursor;
        do{
            if(temp->vertex->pointIndex != pointIndex)
            {
                oppositeVertexes->push_back(temp->vertex);
            }
            temp = temp->next;
        }
        while(temp != cursor);

        if (cursor->pair != nullptr)
            cursor = cursor->pair->next;

    }while(cursor != edge && cursor != nullptr);

    return oppositeVertexes;
}

HalfEdge* Face::findVertex(Vertex* v) const
{
    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    bool found = false;

    do{
        if (tcursor != nullptr)
        {
            if (tcursor->vertex != nullptr)
            {
                if (tcursor->vertex->pointIndex == v->pointIndex)
                    found = true;
            }

            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && !found);

    if (found)
        return tcursor;

    return nullptr;
}

bool Face::inside(int pointIndex) const
{
    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    bool found = true;

    do{
        if (tcursor != nullptr && tcursor->next != nullptr)
        {
            if (tcursor->vertex != nullptr && tcursor->next->vertex != nullptr)
            {
                int a = tcursor->vertex->pointIndex;
                int b = tcursor->next->vertex->pointIndex;
                if (tcursor->vertex->pointIndex != pointIndex && PointRepository::instance()->area2(a, b, pointIndex) > 0)
                    found &= true;
                else
                    found &= false;
            }

            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && !found);

    return found;
}

HalfEdge* Face::getEdgeHoldingPoint(int pointIndex) const
{
    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;

    do{
        if (tcursor != nullptr && tcursor->next != nullptr)
        {
            if (tcursor->vertex != nullptr && tcursor->next->vertex != nullptr)
            {
                int a = tcursor->vertex->pointIndex;
                int b = tcursor->next->vertex->pointIndex;
                if (tcursor->vertex->pointIndex != pointIndex && PointRepository::instance()->area2(a, b, pointIndex) == 0)
                    return tcursor;
            }

            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst);

    return nullptr;
}

HalfEdge* Face::buildFromPoints(const std::vector<int>& pointcloud)
{
    int count = pointcloud.size(); // obtengo la cantidad de puntos
    edge = new HalfEdge();     // se crea el half edge
    edge->face = this;             // se modifica la cara del edge
    edge->face->edge = edge;      // se modifica el edge de la cara
    edge->pair = nullptr;                  //se inicializa el pair  a nullptr

    HalfEdge * cursor = edge;          // se crea un cursor para iterar por el halfedge
    for (int i = 0; i < count; i++)         // por cada punto se hace lo siguiente
    {
        Vertex* vertex = new Vertex();          // se crea el vertice
        vertex->pointIndex = pointcloud[i];     // se cambia el punto
        cursor->vertex = vertex;                // se modifica el vertice del halfedge actual
        vertex->edge = cursor;                      // se activa el half edge del vertice
        if (i < count-1)                        // se genera half edge seguiente solo hasta el penultimo half edge
        {
            cursor = new HalfEdge();
            cursor->face = this;
            cursor->pair = nullptr;
            vertex->edge->next = cursor;
        }
    }
    cursor->next = edge;               // y se agrega el siguiente del ultimo como el primero y se termina.
    return edge;
}

bool Face::operator ==(const Face& face)
{
    int count = face.getVertexesCount();
    if (this->getVertexesCount() != count)
        return false;

    HalfEdge* ffirst = face.findVertex(edge->vertex);
    HalfEdge* fcursor = ffirst;

    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;

    do{
        if (tcursor != nullptr && fcursor != nullptr)
        {
            if (tcursor->vertex != nullptr && fcursor->vertex != nullptr)
            {
                if (!((*tcursor->vertex) == (*fcursor->vertex)))
                    return false;
            }
            else
                return false;

            fcursor = fcursor->next;
            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && fcursor != ffirst);

    if (!(tcursor != tfirst && fcursor != ffirst))
        return false;

    return true;
}

bool Face::intersectWithSegment(int p0, int p1, int SMALL_NUM)
{
    /**
     * Vector    u = S.P1 - S.P0;
    Vector    w = S.P0 - Pn.V0;

    float     D = dot(Pn.n, u);
    float     N = -dot(Pn.n, w);

    if (fabs(D) < SMALL_NUM) {           // segment is parallel to plane
        if (N == 0)                      // segment lies in plane
            return 2;
        else
            return 0;                    // no intersection
    }
    // they are not parallel
    // compute intersect param
    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection

    *I = S.P0 + sI * u;                  // compute segment intersect point
    return 1;
     * */

    Vector3D* u = PointRepository::instance()->getVector3dFromPoints(p0, p1);
    Vector3D* w = PointRepository::instance()->getVector3dFromPoints(edge->vertex->pointIndex, p0);
    Vector3D* n = this->getNormal();
    int D = n->dot(*u);
    int N = n->dot(*w);

    delete n;
    delete w;
    delete u;

    if (SMALL_NUM == -1) {
        vector<int> *pointList = this->getVertexesIndices();

        SMALL_NUM = PointRepository::instance()->generalArea2(*pointList);
    }

    if (abs(D) < SMALL_NUM)
    {
        if (N == 0)
            return true;
        else
            return false;
    }

    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection

    return 1;

}

int Face::getVertexesCount() const
{
    HalfEdge* first = this->edge;
    HalfEdge* cursor = first;
    int count = 0;

    if (first == nullptr)
        return 0;

    do{
        if (cursor != nullptr)
        {
            if (cursor->vertex != nullptr)
            {
                ++count;
            }

            cursor = cursor->next;
        }
    }while(cursor != first);

    return count;
}

vector<int>* Face::getVertexesIndices()
{
    vector<int>* indexes = new vector<int>();
    HalfEdge* first = this->edge;
    HalfEdge* cursor = first;

    if (first == nullptr)
        return indexes;

    do{
        if (cursor != nullptr)
        {
            if (cursor->vertex != nullptr)
            {
                indexes->push_back(cursor->vertex->pointIndex);
            }

            cursor = cursor->next;
        }
    }while(cursor != first);

    return indexes;
}

bool Face::inLeft(Face* nFace)
{
    vector<int>* listV = getVertexesIndices();
    if (listV->size() > 3)
        return false;
    HalfEdge* cursor = nFace->edge;
    int vertexCount = 0, inleftCount = 0, n = listV->size();

    do{
        int a = (*listV)[0];
        int b = (*listV)[1];
        int c = (*listV)[2];
        ++vertexCount;
        Vertex* v = cursor->vertex;
        if (v != nullptr)
        {
            if (PointRepository::instance()->volume6(a, b, c, v->pointIndex) >= 0)
                ++inleftCount;
        }
    }while(cursor != nFace->edge);

    delete listV;

    return vertexCount == inleftCount;
}

bool Face::areIntersected(Face* nFace)
{
    vector<int>* listV = getVertexesIndices();
    if (listV->size() > 3)
        return false;
    HalfEdge* cursor = nFace->edge;
    int vertexCount = 0, inleftCount = 0, n = listV->size();

    do{
        int a = (*listV)[0];
        int b = (*listV)[1];
        int c = (*listV)[2];
         ++vertexCount;
         Vertex* v = cursor->vertex;
         if (v != nullptr)
         {
             if (PointRepository::instance()->volume6(a, b, c, v->pointIndex) >= 0)
                 ++inleftCount;
         }
    }while(cursor != nFace->edge);
    delete listV;
    return (inleftCount > 0 && vertexCount != inleftCount);
}

Vector3D* Face::getNormal()
{
    if(edge == nullptr || edge->next == nullptr)
        return nullptr;
    Vector3D* A = edge->getOrientedVector3D();
    Vector3D* B = edge->next->getOrientedVector3D();

    return &(A->cross(*B));
}
