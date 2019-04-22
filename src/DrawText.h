//
// Created by rkindela on 09-04-19.
//

#ifndef PMTOOL_DRAWTEXT_H
#define PMTOOL_DRAWTEXT_H

#include <cstring>
#include <cstdio>
#include "utils.h"
#include <vector>
#include <GL/glut.h>

class DrawText {

private:
    vector<string> texts;
    char label[256];

public:
    DrawText(){
        this->addText("Hola Mundo Cruel");
        this->addText("Hola Mundo Cruel");
        this->addText("Hola Mundo Cruel");
        this->addText("Hola Mundo Cruel");
    }
    void addText(const string& text){texts.push_back(text);}

    void inline drawString (char *s)
    {
        unsigned int i;
        for (i=0; i<strlen(s); i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
    }

    void render()
    {

        int i;
        double angulo;
        float x,y;
        char *nombre="angulo    sin(angulo)";


        glColor3f(0.0, 0.0, 0.0);
        int ii = 0;
        for(auto str: texts)
        {
            glPushMatrix();
            glLoadIdentity();
//            sprintf(label,"%s", str.c_str());
            x=100;
            y=35*(++ii);
            glRasterPos2f(-900, 600);
            drawString (label);

            glColor3f(1.0, 1.0, 1.0);
            angulo=0.0;

            for (i=0; i< texts.size(); i++)
            {
//                sprintf (label, "%4.1f",texts[i].c_str());
                glRasterPos2f (x,y);
                drawString (label);
                y=y+150;
            }
            glFlush();
            glPopMatrix();
        }
        glutSwapBuffers();
    }
};


#endif //PMTOOL_DRAWTEXT_H
