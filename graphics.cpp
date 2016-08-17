/*
                    ***** graphics.cpp *****

OpenGL graphics program, illustrating use of GLUT and callback functions.

Author: John M. Weiss, Ph.D.
Class:  CSC300 Data Structures
Date:   Fall 2015

Modifications:
*/

// include files
#include <cstdlib>
#include <iostream>
#include <string>
//#include <cmath>

using namespace std;

// the GLUT header automatically includes gl.h and glu.h
#include <GL/glut.h>
#include "graphics.h"
#include "globals.h"


typedef unsigned char byte;

/*
#include "line.h"
#include "ellipse.h"
#include "rectangle.h"
*/
/******************************************************************************/
/*                          useful graphics routines                          */
/******************************************************************************/

// draw a line using the given RGB color value
void DrawLine( float x1, float y1, float x2, float y2, const float color[] )
{
    // glLineWidth( 10 );
    glColor3fv( color );
    glBegin( GL_LINES );
    glVertex2f( x1, y1 );
    glVertex2f( x2, y2 );
    glEnd();
    glFlush();
}

/******************************************************************************/

// draw a rectangle using the given RGB color value
void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] )
{
    glColor3fv( color );
    glBegin( GL_LINE_LOOP );
    glVertex2f( x1, y1 );
    glVertex2f( x2, y1 );
    glVertex2f( x2, y2 );
    glVertex2f( x1, y2 );
    glEnd();
    glFlush();
}

/******************************************************************************/

// draw a filled rectangle using the given RGB color value
void DrawFilledRectangle( float x1, float y1, float x2, float y2, const float color[] )
{
    glColor3fv( color );
    glBegin( GL_POLYGON );
    glVertex2f( x1, y1 );
    glVertex2f( x2, y1 );
    glVertex2f( x2, y2 );
    glVertex2f( x1, y2 );
    glEnd();
    glFlush();
}

/******************************************************************************/

// draw an ellipse of given radii at a specified position using the given RGB color value
void DrawEllipse( float xRadius, float yRadius, int x, int y, const float color[] )
{
    float radius = xRadius < yRadius ? xRadius : yRadius;   // stretch circle into ellipse
    glColor3fv( color );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( x, y, 0 );
    glScalef( xRadius / radius, yRadius / radius, 1.0 );    // by ratio of major to minor axes
    GLUquadricObj *disk = gluNewQuadric();
    gluDisk( disk, radius - 1, radius, int( radius ), 1 );
    gluDeleteQuadric( disk );
    glLoadIdentity();
    glFlush();
}

/******************************************************************************/

// draw an ellipse of given radii at a specified position using the given RGB color value
void DrawFilledEllipse( float xRadius, float yRadius, int x, int y, const float color[] )
{
    float radius = xRadius < yRadius ? xRadius : yRadius;   // stretch circle into ellipse
    glColor3fv( color );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( x, y, 0 );
    glScalef( xRadius / radius, yRadius / radius, 1.0 );    // by ratio of major to minor axes
    GLUquadricObj *disk = gluNewQuadric();
    gluDisk( disk, 0, radius, int( radius ), 1 );
    gluDeleteQuadric( disk );
    glLoadIdentity();
    glFlush();
}

/******************************************************************************/

// write a text string at a specified position using the given RGB color value
void DrawTextString( string text, int x, int y, const float color[] )
{	
    glColor3fv(White);
    glRasterPos2i( x, y );
    for(unsigned int i = 0; i < text.length(); i++)
        glutBitmapCharacter( GLUT_BITMAP_9_BY_15, text[i] );
}

void DrawSector(int s, unsigned int probed, unsigned int area, unsigned int edge)
{
    float dim = area/edge;

    //edge is how many divisions
    //area is length of area in pixels
    //dim is size of one "pixel"

    float x1 = (s % edge) * dim + (glutGet(GLUT_WINDOW_WIDTH)-area)/2.0,
          y1 = (glutGet(GLUT_WINDOW_HEIGHT)-area)/4.0 + (area - (s / edge + 1)*dim),
          x2 = x1 + dim,
          y2 = y1 + dim,
          color[] = { 0.5, 0.5, 1.0 };

    double interval = 3.0;

    if(probed >0 && probed < interval) //blue getting greener
    {
       color[0] = 0.0;
       color[1] = 0.0 + 1.0*(probed/interval);
       color[2] = 1.0 - 1.0*(probed/interval);
    }
    if(probed >=interval && probed < interval*2) // green getting yellower
    {
       color[0] = 0.0 + 1.0*((probed-interval)/interval);
       color[1] = 1.0;
       color[2] = 0.0;
    }
    if(probed >= interval*2 && probed < interval*3)//yellow getting redder
    {
       color[0] = 1.0;
       color[1] = 1.0 - 1.0*((probed-interval*3)/interval);
       color[2] = 0.0;
    }
    if(probed >= interval*3) //red super red
    {
        color[0] = (interval*3)/(probed);
        color[1] = 0.0;
        color[2] = 0.0;
    }

    if(dim > 4)
    {
        DrawFilledRectangle(x1, y1, x2, y2, Black);
        DrawFilledRectangle(x1+1, y1+1, x2-1, y2-1, color);
    }
    else
    {
        DrawFilledRectangle(x1, y1, x2, y2, color);
    }


} 

void drawTexture(float x1, float y1, float x2, float y2, byte* data,
                 float width, float height)
{

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    glLoadIdentity();
    glDisable(GL_LIGHTING);


    GLuint texture;

    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, texture);

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
           glTexCoord2f(0, 0); glVertex2f(x1, y2);
           glTexCoord2f(0, 1); glVertex2f(x1, y1);
           glTexCoord2f(1, 1); glVertex2f(x2, y1);
           glTexCoord2f(1, 0); glVertex2f(x2, y2);
    glEnd();
    glFlush();
}

void DrawSector(int s, byte* data)
{
    int border = 1;
    float height = glutGet(GLUT_WINDOW_HEIGHT) / 10,
          width = glutGet(GLUT_WINDOW_WIDTH) / 25;
    float x1 = (s % 25) * width + border,
          y1 = glutGet(GLUT_WINDOW_HEIGHT) - (s / 25) * height - border,
          x2 = (s % 25) * width + width - border,
          y2 = glutGet(GLUT_WINDOW_HEIGHT) - (s / 25) * height - height + border;

    drawTexture(x1, y1, x2, y2, data, width, height);
    DrawRectangle(x1, y1, x2, y2, Black);
    glFlush();

}

void DrawRing(unsigned int s, unsigned int probed, unsigned int edge)
{
    float color[] = { 0.0, 0.0, 1.0 };

    double interval = 4.0;

    if(probed > 0 && probed < interval) //blue getting greener
    {
       color[0] = 0.0;
       color[1] = 0.0 + 1.0*(probed/interval);
       color[2] = 1.0 - 1.0*(probed/interval);
    }
    if(probed >=interval && probed < interval*2) // green getting yellower
    {
       color[0] = 0.0 + 1.0*((probed-interval)/interval);
       color[1] = 1.0;
       color[2] = 0.0;
    }
    if(probed >= interval*2 && probed < interval*3)//yellow getting redder
    {
       color[0] = 1.0;
       color[1] = 1.0 - 1.0*((probed-interval*3)/interval);
       color[2] = 0.0;
    }
    if(probed >= interval*3) //red to super red
    {
        color[0] = (interval*3)/(probed);
        color[1] = 0.0;
        color[2] = 0.0;
    }

    int x = glutGet(GLUT_WINDOW_WIDTH)/2, y = glutGet(GLUT_WINDOW_HEIGHT)/2;

    double radius = ((double)(edge - s)/(edge))*(x);

    //hashpipe (not a drug reference) #||#
    //x = ((-0.0027)*radius*radius + 2.1*radius) + glutGet(GLUT_WINDOW_WIDTH)/2;
    //y = ((-0.0012)*radius*radius + .7*radius) + glutGet(GLUT_WINDOW_HEIGHT)/2;

    //x = x * (edge/1361.0);
    //y = y * (edge/1361.0);


    //hashcone
    y = (0.001)*radius*radius - 2*radius + glutGet(GLUT_WINDOW_HEIGHT)*(.9);

    //experimenting
    //y = (-1.0)*sqrt((radius*radius - 500)) + glutGet(GLUT_WINDOW_HEIGHT)*(.9);

    DrawEllipse(radius, radius*0.7, x, y, color);

}
