//
// Created by rkindela on 19-07-18.
//

#include "Polyhedron.h"

Polyhedron::Polyhedron():faces(), vertexes(), edges()
{

}

vector<int>* Polyhedron::getLink(int point)
{
    Vertex* v = this->getVertex(point);
    vector<Vertex*>* link = v->getLink();

    std::vector<Vertex*>::iterator begin = link->begin();
    std::vector<Vertex*>::iterator end = link->end();

    vector<int>* linkIndexes = new vector<int>();

    for(auto i = begin; i != end; ++i)
    {
        linkIndexes->push_back((*i)->pointIndex);
    }

    delete link;

    return linkIndexes;
}

int Polyhedron::addVertex(int point)
{
    Vertex* v = new Vertex();
    v->pointIndex = point;
    this->vertexes.push_back(v);
    return this->vertexes.size()-1;
}

int Polyhedron::addFace()
{
    Face* f = new Face();
    this->faces.push_back(f);
    return this->faces.size()-1;
}

int Polyhedron::addEdge(int init, int final)
{
    HalfEdge* edge = new HalfEdge();
    edge->vertex = this->getVertex(init);
    edge->vertex->edge = edge;
    edge->face = this->getFace(final);
    edges.push_back(edge);
    return edges.size()-1;
}

Vertex* Polyhedron::getVertex(int pos)
{
    if (pos < 0 || pos > vertexes.size())
        return nullptr;

    return vertexes[pos];
}

Vertex* Polyhedron::getVertex(int pos) const
{
    if (pos < 0 || pos > vertexes.size())
        return nullptr;

    return vertexes[pos];
}

POINT* Polyhedron::getPoint(int pos)
{
    Vertex* v = this->getVertex(pos);
    if(v == nullptr)
        return nullptr;

    return PointRepository::instance()->getPoint(v->pointIndex);
}

POINT* Polyhedron::getPoint(int pos) const
{
    Vertex* v = this->getVertex(pos);
    if(v == nullptr)
        return nullptr;

    return PointRepository::instance()->getPoint(v->pointIndex);
}

Face* Polyhedron::getFace(int pos)
{
    if(0 > pos || faces.size() < pos)
        return nullptr;
    return faces[pos];
}

Face* Polyhedron::getFace(int pos) const
{
    if(0 > pos || faces.size() < pos)
        return nullptr;
    return faces[pos];
}

HalfEdge* Polyhedron::getEdge(int pos)
{
    if(0 > pos || edges.size() < pos)
        return nullptr;
    return edges[pos];
}

HalfEdge* Polyhedron::getEdge(int pos) const
{
    if(0 > pos || edges.size() < pos)
        return nullptr;
    return edges[pos];
}

int Polyhedron::getPointCount() const
{
    return vertexes.size();
}

int Polyhedron::getFaceCount() const
{
    return faces.size();
}

int Polyhedron::getEdgeCount() const
{
    return edges.size();
}

bool Polyhedron::intersect(const Polyhedron& newPoly)
{
    /**
     *
Idea basica para generalizar el algoritmo de interseccion de poligonos:

	Supongamos que las caras  P y Q
estan orientados ccw y que A y B son los vectores normales de P y Q, respectivamente.

    El algoritmo tiene a A y B "cazandose" uno a otro
respectivamente, ajustando sus velocidades de tal forma
de encontrarse cada vez que P y Q se intersectan.

A y B se representan como vectores y lo clave aqui son las
reglas para avanzar A y B.

La estructura basica del algoritmo es la siguiente:

     Se avanzan las caras P y Q segun condiciones geometricas. y cuando se detecte una interseccion se termina.
     al final es necesario chequear que P no contenga a Q y viceversa
     * */

    Face* P = faces[0];
    Face* Q = newPoly.faces[0];

    int itP = 0, itQ = 0;

    int n = faces.size(), m = newPoly.faces.size();                  /** indices en P y Q */
    Vector3D *A, *B;            // aristas orientadas en P y Q
    int cross;                  // AxB
    bool bHA, aHB;              // b in H(A), a in H(B)

    /**Inicializacion de variables*/

    do
    {
        /**Calculo de variables claves*/

        A = P->getNormal();
        B = Q->getNormal();

        cross = A->detFromCross(*B); // en 2D AxB = C y C tiene la forma (0,0, Cz), por tanto AxB == Cz
        bHA = P->inLeft(Q); // leftOn
        aHB = Q->inLeft(P); // leftOn

        if (P->areIntersected(Q))
        {
            return true;
        }

        /**Advances Rules*/
        if (cross >= 0)
        {
            if (bHA) {
                P = faces[(++itP)%n];
            }
            else{
                Q = newPoly.faces[(++itQ)%m];
            }
        } else
        {
            if (aHB) {
                Q = newPoly.faces[(++itQ)%m];
            }
            else{
                P = faces[(++itP)%n];
            }
        }
    }
    while((itP < n) || (itQ < m));

    /**Casos especiales*/
    return this->contains(newPoly) || newPoly.contains(*this);
}

bool Polyhedron::isRegular()
{
    int nedges = edges.size();
    if(nedges == 0)
        return false;
    Vector3D* v = edges[0]->getOrientedVector3D();
    int side = v->squareNorm(), oldside = side;
    for (int i = 1; i < nedges; i++)
    {
        v = edges[i]->getOrientedVector3D();
        side = v->squareNorm();
        if (side != oldside)
            return false;
        oldside = side;
    }
    return true;
}

bool Polyhedron::isSimple()
{
    //check simplicity of polyhedron using Euler characteristics
    // V - F + E = 2
    return true;
}

bool Polyhedron::isConvex()
{
    int count = vertexes.size();
    int numfaces = faces.size();
    if (numfaces == 0 || count == 0)
        return false;

    POINT* centroid = new POINT();
    for(int i = 0; i<count;i++) // find the centroid of polyhedron
    {
        Vertex* v = vertexes[i];
        POINT* p = PointRepository::instance()->getPoint(v->pointIndex);
        centroid->x += p->x;
        centroid->y += p->y;
        centroid->z += p->z;
    }
    int div = count == 0 ? 1 : count;
    centroid->x /= div;
    centroid->y /= div;
    centroid->z /= div;

    PointRepository::instance()->add(centroid); // add to repository and count is it index because centroid is the last point

    bool sign = true, oldsign = true;
    Face* f = faces[0];                 // as all face is oriented, we just need the first three vertex to check
    int a = f->edge->vertex->pointIndex;
    int b = f->edge->next->vertex->pointIndex;
    int c = f->edge->next->next->vertex->pointIndex;
    int d = count;

    sign = (PointRepository::instance()->volume6(a, b, c, d) >= 0);
    oldsign = sign;
    for(int i = 1; i < numfaces;i++)        // for any face we check the volume orientation all of this will be the same sign
    {
        Face* f = faces[i];                 // as all face is oriented, we just need the first three vertex to check
        int a = f->edge->vertex->pointIndex;
        int b = f->edge->next->vertex->pointIndex;
        int c = f->edge->next->next->vertex->pointIndex;

        sign = PointRepository::instance()->volume6(a, b, c, d) >= 0;
        if (sign != oldsign) {
            PointRepository::instance()->remove(d);
            return false;
        }
        oldsign = sign;
    }
    return true;
}

bool Polyhedron::inside(int npoint)
{
    return pointIntersections(npoint) % 2 != 0;
}

int Polyhedron::pointIntersections(int npoint)
{
    /**
     *  1) Draw a horizontal line from npoint to infinity

        1) Count the number of times the line intersects with polyhedron faces.

        2) A point is inside the polyhedron if either count of intersections is odd or
           point lies on a face of polyhedron.  If none of the conditions is true, then
           point lies outside.polyhedron
     */
    int nfaces = faces.size();
    if (nfaces == 0)
        return false;

    POINT* point = this->getPoint(npoint);
    PointRepository::instance()->add(new POINT(2147483647, point->y));
    int end = PointRepository::instance()->getPointCount()-1;

    int intersectionsNumber = 0;
    for (int i = 0; i < nfaces; i++)
    {
        Face* f = faces[i];
        if (f->intersectWithSegment(npoint, end))
            ++intersectionsNumber;
    }

    return intersectionsNumber;
}

BoundingSphereOctree* Polyhedron::getBoundingSphere() const
{
    return nullptr;
}

bool Polyhedron::contains(const Polyhedron& p) const
{
   return this->getBoundingSphere()->hasInside(p.getBoundingSphere());
}

bool Polyhedron::left(int npoint)
{
    return (pointIntersections(npoint) % 2) == 0;
}

bool Polyhedron::rigth(int npoint)
{
    return pointIntersections(npoint) == 0;
}

float Polyhedron::area2()
{

}

std::vector<Polyhedron*> Polyhedron::triangulate()
{

}

vector<Polyhedron*>* Polyhedron::getConvexHull()
{

}
vector<Polyhedron*>* Polyhedron::giftWrapping(std::vector<int> pointcloud)
{

}
vector<Polyhedron*>* Polyhedron::grahamScan(std::vector<int> pointcloud)
{

}

std::ostream& operator<<(std::ostream& os, const Polyhedron* poly)
{

}

std::ostream& operator<<(std::ostream& os, const Polyhedron& poly)
{

}

bool Polyhedron::isEqual(Polyhedron& polyB)
{

}
