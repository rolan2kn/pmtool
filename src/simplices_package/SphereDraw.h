//
// Created by rkindela on 28-01-19.
//

#ifndef PMTOOL_SPHEREDRAW_H
#define PMTOOL_SPHEREDRAW_H

#include <cmath>
#include <GL/glut.h>

class SphereDraw {

public:
    static void execute(float radio, int nlatitud, int nlongitud)
    {
        const float pi = 3.14159265358979323846;
        float inct, incf;
        int i, j;
        float vertice[3];
        inct = 2 * pi / nlatitud;
        incf = pi / nlatitud;
        for( i = 0; i < nlatitud; i++ )
        {
            glBegin( GL_LINE_STRIP );
            vertice[0] = vertice[1] = 0;
            vertice[2] =- radio;
            glVertex3fv( vertice );
            for( j = 1; j < nlongitud-1; j++ )
            {
                vertice[0] = radio * cos(i*inct) * cos(j*incf-0.5*pi);
                vertice[1] = radio * sin(i*inct) * cos(j*incf-0.5*pi);
                vertice[2] = radio * sin(j*incf-0.5*pi);
                glVertex3fv( vertice );
                vertice[0] = radio*cos((i+1)*inct)*cos(j*incf-0.5*pi);
                vertice[1] = radio*sin((i+1)*inct)*cos(j*incf-0.5*pi);
                glVertex3fv(vertice);
            }
            vertice[0] = vertice[1] = 0;
            vertice[2] = radio;
            glVertex3fv( vertice );
        }
        glEnd();
    }
};


#endif //PMTOOL_SPHEREDRAW_H
