#include "mainglwidget.h"

#include "graphics/modelling.h"
#include "graphics/graphicssystem.h"
#include "physics/actions.h"

MainGLWidget::MainGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
}

void MainGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        event->ignore();
    else
        emit keyPressed(event->key());
}

void MainGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        event->ignore();
    else
        emit keyReleased(event->key());
}

void MainGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event->pos().x(), event->pos().y());
}

void MainGLWidget::initializeGL()
{
    if(!GraphicsSystem::instance()->LoadGLTextures())   // Jump To Texture Loading Routine
        throw "MainGLWidget::initializeGL: GraphicsSystem::instance()->LoadGLTextures() failed!";

    glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
    glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    // dodatkowe: test alpha, blending (translucant)
//    glEnable(GL_ALPHA_TEST);
//    glEnable(GL_BLEND );
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // poczatkowe oswietelenie
    GLfloat LightAmbientGlobal[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat LightAmbientGlobalPosition[] = { 5.0f, 5.0f, 10.0f, 1.0f }; // TODO ?
    glLightfv( GL_LIGHT0, GL_DIFFUSE, LightAmbientGlobal );

    // Enable Light One
    glEnable(GL_LIGHT1);
}

void MainGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(0.0, 1.0, 0.0, 0.0);

    //SetLights();
    setLook();
    DrawGLMap(World::instance()->getMap());
    //MovingCamera();
}

void MainGLWidget::setLook()
{
    Position &playerPos = World::instance()->getPlayer()->position();
    gluLookAt(playerPos.x, playerPos.y, playerPos.z,
              playerPos.x + playerPos.lx, playerPos.y + playerPos.ly, playerPos.z + playerPos.lz,
              0.0, 1.0, 0.0);
}






















/********* FROM VS PROJECT ************************************************************************/
/********* FROM VS PROJECT ************************************************************************/
/********* FROM VS PROJECT ************************************************************************/
/********* FROM VS PROJECT ************************************************************************/

#include <stdio.h>
#include "sound/sound.h"
#include "utils/structures.h"



// zmienna ruchomego oswietlenia
extern float x_light;
extern float y_light;
extern float z_light;
int n_lights; // zmienne swiatel obiektow, ile ich jest
extern int n_lists;
// obce
extern GLfloat	yrot;				// Y Rotation
extern GLfloat lookupdown ;
extern float heading;
float xpos;
float zpos;


//smieci initGL
//glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    //glMateriali(GL_FRONT, GL_SHININESS, 96);
    //glEnable(GL_LIGHT0);
    //quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
    //gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
    //gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )

    //float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, specReflection);

int DrawGLScene()									// Here's Where We Do All The Drawing
{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
//    glLoadIdentity();									// Reset The View
//    /******new motion variables****/
//    xpos *= 0.8f;
//    zpos *= 0.8f;
//    xtrans2 += xpos/10;
//    ztrans2 += zpos/10;
//    // tu wczesniej byly deklaracje - czy to duza zmiana?
//    xtrans = -xtrans2;
//    ztrans = -ztrans2;
//    zprot*=0.9;
//    _heading += zprot;
//    heading = mouse_xpos_cursor + _heading;
//    yrot = heading;
//    //ytrans = ytrans-walkbias-0.35f;
//    GLfloat sceneroty = 360.0f - yrot;

//    glRotatef(lookupdown,1.0f,0,0);
//    glRotated(mouse_ypos_cursor,1.0f,0,0);
//    glRotatef(sceneroty,0,1.0f,0);

//    glTranslatef(xtrans,ytrans,ztrans);

//    SetLights();

//    /****end of new variables****/
//    DrawGLMap( current );

//    glBegin(GL_TRIANGLES);
//        glNormal3f( 0, 1, 0 );
//        glVertex3f(x_light,y_light,z_light);
//        glVertex3f(x_light,y_light,z_light-0.05f);
//        glVertex3f( x_light, y_light-0.05f, z_light-0.05f);
//    glEnd();

//                // dzwieki ruchu gracza - w jakim miejscu?
//                MovingCamera();
//    //gluCylinder(quadratic,0.2f,0.2f,3.0f,32,32);	// Draw Our Cylinder

//    return TRUE;										// Everything Went OK
}
