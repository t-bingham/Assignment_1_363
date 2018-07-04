//  ========================================================================
//  Author: Thomas Bingham
//  COSC363 Assignment 1
//  Date last Edited: 24/03/2018
//
//  ========================================================================

#include <cmath>
#include <math.h>
#include <GL/freeglut.h>
#include "loadTGA.h"
#include <iostream>

float theta = 1;
float wing = 0;
float flag = 1;
float wasp_theta = 1.5;
float wasp_flag = 1;
float wasp_wing = 30;
float angle=0, look_x, look_z=-5., look_y = 0, eye_x=0, eye_y = 15, eye_z=40;  //Camera parameters
#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
int camFlag = 0;
float charRotate = 0.0;
float shift = 0.0;
int multiplier = 3;

GLuint texId[10];


//-- SkyTex ------------------------------------------------------------
void loadGLTextures()               // Load bitmaps And Convert To Textures
{
    glGenTextures(10, texId);        // Create texture ids
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("lt.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("ft.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("rt.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("bk.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    loadTGA("dn.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** inner walls ***
    glBindTexture(GL_TEXTURE_2D, texId[6]);  //Use this texture
    loadTGA("innerWall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // *** outer walls ***
    glBindTexture(GL_TEXTURE_2D, texId[7]);  //Use this texture
    loadTGA("outerWall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // *** floor ***
    glBindTexture(GL_TEXTURE_2D, texId[8]);  //Use this texture
    loadTGA("floor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[9]);  //Use this texture
    loadTGA("Wood.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//-- Torch -------------------------------------------------------------

const int N = 10;
float vx[N] = {0, 8, 8, 6, 4, 4, 3.95, 4.5, 4.5, 0};
float vy[N] = {5, 0, 10, 11, 15, 45, 45.5, 46.5, 50, 50};
float vz[N] = {0};

//-- Surface broken into quads -----------------------------------------
void floor()
{
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glColor4f(0.3, 0.3, 0.3, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);

    //The floor is made up of several tiny squares on a 100 x 93 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -50; i < 50; i++)
    {
        for(int j = -80;  j < 13; j++)
        {
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glVertex3f(i+1, 0, j+1);
            glVertex3f(i+1, 0, j);
        }
    }
    glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

//-- Building ------------------------------------------------------------

void textureBuilding()
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(0, 0, 0);   //replace with a texture


//---- Outer Walls -----------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, texId[7]);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glBegin(GL_QUADS);
    ////////////////////// BACK WALL ///////////////////////

    glTexCoord2f(0.0, 2.0);
    glVertex3f(-10, 7.5, -21);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-10, 2.5, -21);
    glTexCoord2f(12.0, 0.0);
    glVertex3f(10, 2.5, -21);
    glTexCoord2f(12.0, 2.0);
    glVertex3f(10, 7.5, -21);

    ////////////////////// FRONT WALLS ///////////////////////

     glTexCoord2f(0.0, 0.6);
     glVertex3f(10, 7.5, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(10, 6, 0);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(-10, 6, 0);
     glTexCoord2f(12.0, 0.6);
     glVertex3f(-10, 7.5, 0);

     glTexCoord2f(0.0, 1.4);
     glVertex3f(10, 6, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(10, 2.5, 0);
     glTexCoord2f(5.0, 0.0);
     glVertex3f(2, 2.5, 0);
     glTexCoord2f(5.0, 1.4);
     glVertex3f(2, 6, 0);

     glTexCoord2f(0.0, 2.0);
     glVertex3f(-10, 6, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-10, 2.5, 0);
     glTexCoord2f(5.0, 0.0);
     glVertex3f(-2, 2.5, 0);
     glTexCoord2f(5.0, 2.0);
     glVertex3f(-2, 6, 0);

    ////////////////////// LEFT WALL ///////////////////////
     glTexCoord2f(0.0, 2.0);
     glVertex3f(-10, 7.5, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-10, 2.5, 0);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(-10, 2.5, -21);
     glTexCoord2f(12.0, 2.0);
     glVertex3f(-10, 7.5, -21);


    ////////////////////// RIGHT WALL ///////////////////////
     glTexCoord2f(0.0, 2.0);
     glVertex3f(10, 7.5, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(10, 2.5, 0);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(10, 2.5, -21);
     glTexCoord2f(12.0, 2.0);
     glVertex3f(10, 7.5, -21);

//---- Door Walls ------------------------------------------------------
     glTexCoord2f(0.0, 0.2);
     glVertex3f(2, 6, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(2, 6, -0.5);
     glTexCoord2f(1.20, 0.0);
     glVertex3f(-2, 6, -0.5);
     glTexCoord2f(1.2, 0.2);
     glVertex3f(-2, 6, -0);

     glTexCoord2f(0.0, 0.2);
     glVertex3f(2, 6, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(2, 6, -0.5);
     glTexCoord2f(1.20, 0.0);
     glVertex3f(2, 2.5, -0.5);
     glTexCoord2f(1.2, 0.2);
     glVertex3f(2, 2.5, -0);

     glTexCoord2f(0.0, 0.2);
     glVertex3f(-2, 6, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-2, 6, -0.5);
     glTexCoord2f(1.20, 0.0);
     glVertex3f(-2, 2.5, -0.5);
     glTexCoord2f(1.2, 0.2);
     glVertex3f(-2, 2.5, -0);

     glEnd();
//---- Inner Walls -----------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, texId[6]);
    glBegin(GL_QUADS);

    ////////////////////// BACK WALL ///////////////////////

    glTexCoord2f(0.0, 2.0);
    glVertex3f(-9.5, 7.5, -19.5);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-9.5, 2.5, -19.5);
    glTexCoord2f(12.0, 0.0);
    glVertex3f(9.5, 2.5, -19.5);
    glTexCoord2f(12.0, 2.0);
    glVertex3f(9.5, 7.5, -19.5);

    ////////////////////// FRONT WALLS ///////////////////////

     glTexCoord2f(0.0, 0.6);
     glVertex3f(9.5, 7.5, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(9.5, 6, -0.5);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(-9.5, 6, -0.5);
     glTexCoord2f(12.0, 0.6);
     glVertex3f(-9.5, 7.5, -0.5);

     glTexCoord2f(0.0, 1.4);
     glVertex3f(9.5, 6, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(9.5, 2.5, -0.5);
     glTexCoord2f(5.0, 0.0);
     glVertex3f(2, 2.5, -0.5);
     glTexCoord2f(5.0, 1.4);
     glVertex3f(2, 6, -0.5);

     glTexCoord2f(0.0, 2.0);
     glVertex3f(-9.5, 6, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-9.5, 2.5, -0.5);
     glTexCoord2f(5.0, 0.0);
     glVertex3f(-2, 2.5, -0.5);
     glTexCoord2f(5.0, 2.0);
     glVertex3f(-2, 6, -0.5);

    ////////////////////// LEFT WALL ///////////////////////
     glTexCoord2f(0.0, 2.0);
     glVertex3f(-9.5, 7.5, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-9.5, 2.5, 0);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(-9.5, 2.5, -19.5);
     glTexCoord2f(12.0, 2.0);
     glVertex3f(-9.5, 7.5, -19.5);


    ////////////////////// RIGHT WALL ///////////////////////
     glTexCoord2f(0.0, 2.0);
     glVertex3f(9.5, 7.5, 0);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(9.5, 2.5, 0);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(9.5, 2.5, -19.5);
     glTexCoord2f(12.0, 2.0);
     glVertex3f(9.5, 7.5, -19.5);

     glEnd();

    //---- Floor -----------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glBegin(GL_QUADS);

    //~ ////////////////////// FLOOR ///////////////////////

    //~ glTexCoord2f(0.0, 2.0);
    //~ glVertex3f(-9.5, 3, -19.5);
    //~ glTexCoord2f(0.0, 0.0);
    //~ glVertex3f(9.5, 3, -19.5);
    //~ glTexCoord2f(12.0, 0.0);
    //~ glVertex3f(9.5, 3, -0.5);
    //~ glTexCoord2f(12.0, 2.0);
    //~ glVertex3f(-9.5, 3, -0.5);


    ////////////////////// RAMP ///////////////////////
     glTexCoord2f(0.0, 2.0);
     glVertex3f(-2, 3, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(2, 3, -0.5);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(2, 2.5, 0);
     glTexCoord2f(12.0, 2.0);
     glVertex3f(-2, 2.5, 0);

     glEnd();

    glBindTexture(GL_TEXTURE_2D, texId[9]);
    glBegin(GL_TRIANGLES);


    ////////////////////// ROOF ///////////////////////
    //inner
     glTexCoord2f(0.0, 2.0);
     glVertex3f(9.5, 7.5, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-9.5, 7.5, -0.5);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(0, 10, -10);

     glTexCoord2f(0.0, 2.0);
     glVertex3f(9.5, 7.5, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(9.5, 7.5, -19.5);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(0, 10, -10);

     glTexCoord2f(0.0, 2.0);
     glVertex3f(-9.5, 7.5, -0.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-9.5, 7.5, -19.5);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(0, 10, -10);

     glTexCoord2f(0.0, 2.0);
     glVertex3f(9.5, 7.5, -19.5);
     glTexCoord2f(0.0, 0.0);
     glVertex3f(-9.5, 7.5, -19.5);
     glTexCoord2f(12.0, 0.0);
     glVertex3f(0, 10, -10);

     //outer

     glEnd();

     glDisable(GL_TEXTURE_2D);


}

//-- Skybox ------------------------------------------------------------
void skybox(){
    glEnd();
    glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000,  0, 1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-1000, 0., -1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-1000, 1000., -1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000, 1000, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000,  0, -1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, 0., -1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 1000, -1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000,  1000, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(1000,  0, -1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, 0, 1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(1000,  1000,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f( 1000, 0, 1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-1000, 0,  1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-1000, 1000,  1000);
  glTexCoord2f(0.0,1.0);
  glVertex3f( 1000, 1000, 1000);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000, 1000, -1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, 1000,  -1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000, 1000, 1000);
  glEnd();

  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000, 0., 1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, 0.,  1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 0., -1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000, 0., -1000);
  glEnd();

  glDisable(GL_TEXTURE_2D);

}

//----------------------------------------------------------------------


void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}
//-- Staff -----------------------------------------------------------
void staff()
{
    float black[4] = {0};
    glNormal3f(0.0, 1.0, 0.0);
    glRotatef(-85, 0, 0, 1);
    glPushMatrix();
        glMaterialf(GL_FRONT, GL_SHININESS, 100);
        glTranslatef(2, 0, 0);
        glColor3f(0.4, 0.4, 0.4);
        glScalef(10, 0.5, 0.5);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glMaterialf(GL_FRONT, GL_SHININESS, 100);
        glMaterialfv(GL_FRONT, GL_SPECULAR, black);
        glColor3f(0, 0 , 0);
        glTranslatef(-2.5, 0, 0);
        glRotatef(90, 0, 1, 0);
        glScalef(1, 0.5, 0.5);
        glutSolidTorus(0.75, 1, 100, 50);
    glPopMatrix();
    glPushMatrix();
        glMaterialf(GL_FRONT, GL_SHININESS, 10);
        glColor3f(0.9, 0.9, 0.9);
        glTranslatef(-2.5, 0, 0);
        glScalef(1.5, 0.5, 0.5);
        glutSolidSphere(1, 100, 100);
    glPopMatrix();
    glPushMatrix();
        glMaterialf(GL_FRONT, GL_SHININESS, 10);
        glColor3f(0.9, 0.9, 0.9);
        glTranslatef(-4, 0, 0);
        glutSolidSphere(1, 100, 100);
    glPopMatrix();
}

//-- Solar System ------------------------------------------------------
void solarSystem()
{
    glPushMatrix(); //Sun
        glColor3f(1, 1, 0);
        glRotatef(13.5*theta*multiplier, 0, 1, 0);
        glMaterialf(GL_FRONT, GL_SHININESS, 80);
        glutSolidSphere(6, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Mercury
        glRotatef(4.14*theta*multiplier, 0, 1, 0);
        glTranslatef(0, 0, -9);
        glColor3f(0.5, 0.5, 0.5);
        glRotatef(6.2*theta*multiplier, 0, 1, 0);
        glutSolidSphere(0.19, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Venus
        glRotatef(1.42*theta*multiplier, 0, 1, 0);
        glTranslatef(0, 0, -12);
        glColor3f(1, 0.5, 0.5);
        glRotatef(3.14*theta*multiplier, 0, 1, 0);
        glutSolidSphere(0.48, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Earth
        glRotatef(theta*multiplier, 0, 1, 0);
        glTranslatef(0, 0, -15);
        glColor3f(0, 1, 0);
        glRotatef(365*theta*multiplier, 0, 1, 0);
        glutSolidSphere(0.5, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Mars
        glRotatef(0.53*theta*multiplier, 0, 1, 0);
        glTranslatef(0, 0, -17);
        glColor3f(1, 0, 0);
        glRotatef(365*theta*3, 0, 1, 0);
        glutSolidSphere(0.27, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Jupiter
        glRotatef(0.085*theta*multiplier, 0, 1, 0);
        glColor3f(1, 0.3, 0.3);
        glTranslatef(0, 0, -25);
        glRotatef(973*theta*multiplier, 0, 1, 0);
        glutSolidSphere(5.5, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Saturn
        glRotatef(0.033*theta*multiplier, 0, 1, 0);
        glTranslatef(0, 0, -35);
        glColor3f(1, 0.8, 0.3);
        glRotatef(834*theta*multiplier, 0, 1, 0);
        glutSolidSphere(4.6, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Uranus
        glRotatef(0.011*theta*multiplier, 0, 1, 0);
        glColor3f(0, 1, 1);
        glTranslatef(0, 0, -40);
        glRotatef(515*theta*3, 0, 1, 0);
        glutSolidSphere(2, 100, 100);
    glPopMatrix();
    glPushMatrix(); //Neptune
        glRotatef(0.0061*theta*multiplier, 0, 1, 0);
        glColor3f(0, 0.5, 0.5);
        glTranslatef(0, 0, -45);
        glRotatef(547*theta*multiplier, 0, 1, 0);
        glutSolidSphere(1.95, 100, 100);
    glPopMatrix();
}

//-- Bird ------------------------------------------------------------
void bird()
{

    //Wings
    glRotatef(-90, 0, 1, 0);
    glColor4f(0.04, 0.005, 0, 1.0);
    glTranslatef(0, 0.5*sin(4.5*(theta*(M_PI/180))), 0);
    glPushMatrix();
      glRotatef(-wing, 1, 0, 1.5);
      glTranslatef(1.5, 0, 0);
      glRotatef(20, 0, 1, 0);
      glScalef(3.0, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
      glScalef(-1, 1, 1);
      glRotatef(-wing, 1, 0, 1.5);
      glTranslatef(1.5, 0, 0);
      glRotatef(20, 0, 1, 0);
      glScalef(3.0, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    //Body
    glPushMatrix();
      glColor4f(0.04, 0.005, 0, 1.0);
      glScalef(-1.5, -1.0, -1.5);
      glutSolidCone(0.5, 2.0, 100, 100);
    glPopMatrix();

    //Face
    glPushMatrix();
      glTranslatef(0, 0, 0.5);
      glColor4f(1, 0, 0, 1.0);
      glutSolidSphere(0.75, 100, 100);
      glTranslatef(0, 0, 0.5);
      glColor4f(0.6, 0.6, 0.4, 1.0);
      glutSolidCone(0.5, 1.0, 100, 100);//beak
    glPopMatrix();

}

//--- Wasp -----------------------------------------------------------
void wasp()
{

    //Wings
    glTranslatef(-1, -2, -0.5);
    glScalef(0.2, 0.2, 0.2);
    glRotatef(-90, 0, 1, 0);
    glColor4f(1, 1, 1, 1.0);
    glPushMatrix();
      glRotatef(-wasp_wing, 0.5, 0, 1.5);
      glTranslatef(1.5, 0, 0);
      glRotatef(20, 0, 1, 0);
      glScalef(1.5, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
      glScalef(-1, 1, 1);
      glRotatef(-wasp_wing, 0.5, 0, 1.5);
      glTranslatef(1.5, 0, 0);
      glRotatef(20, 0, 1, 0);
      glScalef(1.5, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
      glRotatef(wasp_wing, 0.5, 0, 1.5);
      glTranslatef(0.75, 0, -0.5);
      glRotatef(20, 0, 1, 0);
      glScalef(0.75, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
      glScalef(-1, 1, 1);
      glRotatef(wasp_wing, 0.5, 0, 1.5);
      glTranslatef(0.75, 0, -0.5);
      glRotatef(20, 0, 1, 0);
      glScalef(0.75, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();

    //Thorax
    glPushMatrix();
      glColor4f(0, 0, 0, 1.0);
      glutSolidSphere(0.8, 100, 100);
      glTranslatef(0, 0, 0.8);
    glPopMatrix();

    //Body
    glPushMatrix();
      glColor4f(0.8, 0.8, 0.0, 1.0);
      glTranslatef(0, 0, 1);
      glScalef(-1.5, -1.0, -1.5);
      glRotatef(-95, 1, 0, 0);
      glutSolidCone(0.5, 3.0, 100, 100);
    glPopMatrix();

    //Face
    glPushMatrix();
      glColor4f(0.8, 0.8, 0.0, 1.0);
      glTranslatef(0, 0, 1.3);
      glutSolidSphere(0.5, 100, 100);//head
    glPopMatrix();

}

void characterModel()
{
    //Wings
    glRotatef(180, 0, 1, 0);
    glColor4f(0.04, 0.005, 0, 1.0);
    glPushMatrix();
      glRotatef(-wing, 1, 0, 1.5);
      glTranslatef(1.5, 0, 0);
      glRotatef(20, 0, 1, 0);
      glScalef(3.0, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
      glScalef(-1, 1, 1);
      glRotatef(-wing, 1, 0, 1.5);
      glTranslatef(1.5, 0, 0);
      glRotatef(20, 0, 1, 0);
      glScalef(3.0, 0.25, 1);
      glutSolidCube(1.0);
    glPopMatrix();
    //Body
    glPushMatrix();
      glColor4f(0.04, 0.005, 0, 1.0);
      glScalef(-1.5, -1.0, -1.5);
      glutSolidCone(0.5, 2.0, 100, 100);
    glPopMatrix();

    //Face
    glPushMatrix();
      glTranslatef(0, 0, 0.5);
      glColor4f(1, 0, 0, 1.0);
      glutSolidSphere(0.75, 100, 100);
      glTranslatef(0, 0, 0.5);
      glColor4f(0.6, 0.6, 0.4, 1.0);
      glutSolidCone(0.5, 1.0, 100, 100);//beak
    glPopMatrix();
}

//---------------------------------------------------------------------
void initialize(void)
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
//  Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    loadGLTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 5000.0);   //Perspective projection
}

//-------------------------------------------------------------------
void display(void)
{
   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)

   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
   glDisable(GL_LIGHTING);
   glPushMatrix();
   glTranslatef(0, -500, 0);
   skybox();
   glPopMatrix();
   glEnable(GL_LIGHTING);

   glDisable(GL_LIGHTING);
   glTranslatef(0, 10, 0);
   glPushMatrix();
   glTranslatef(0, -20, 15);
   glScalef(5, 5, 5);
   textureBuilding();
   glPopMatrix();
   glEnable(GL_LIGHTING);

   glPushMatrix();
      glTranslatef(0, -5, 0);
      floor();
   glPopMatrix();
   glPushMatrix(); //Displaying bird
      glTranslatef(-15, 2, -30);
      glRotatef(theta, 0, 1, 0);
      glRotatef(10, 0, 1, 0);
      glRotatef(90, 0, 1, 0);
      glTranslatef(0, 2, -10);
      glRotatef(30, 1, 0, 0);
      bird();
   glPopMatrix();


   glPushMatrix(); //Displaying Wasp Swarm
      glTranslatef(20, 10, -30);
      glRotatef(theta*3.5, 0, 1, 0);
      glRotatef(theta*2.5, 0.2, 0, 0);
      glRotatef(theta*2, 0, 0, 0.3);
      glPushMatrix();
          glPushMatrix();
             glTranslatef(0, 2, -5);
             glRotatef(theta, 0, 1, 0);
             glTranslatef(-1, 0, 0);
             wasp();
          glPopMatrix();
          glPushMatrix();
             glTranslatef(0, 2, -5);
             glRotatef(theta, 1, 0, 0);
             glTranslatef(0, 0, 1);
             wasp();
          glPopMatrix();
          glPushMatrix();
             glTranslatef(0, 2, -5);
             glRotatef(theta, 0, 1, 0);
             glTranslatef(1, 0, 0);
             wasp();
          glPopMatrix();
          glPushMatrix();
             glTranslatef(0, 2, -5);
             glRotatef(theta, 0, 0, 1);
             glTranslatef(0, 1, 0);
             wasp();
          glPopMatrix();
          glPushMatrix();
             glTranslatef(0, 2, -5);
             glRotatef(theta, 0, 0, 1);
             glTranslatef(0, -1, 0);
             wasp();
          glPopMatrix();
          glPushMatrix();
             glTranslatef(0, 2, -5);
             glRotatef(theta, 1, 0, 0);
             glTranslatef(0, 0, -1);
             wasp();
          glPopMatrix();
      glPopMatrix();
   glPopMatrix();

   glPushMatrix();//Displaying Torch
    glTranslatef(0, 0, -50);
    glColor4f (0.3, 0, 0.35, 1.0);
    float wx[N], wy[N], wz[N];
    float angStep = -10.0*3.1415926/180.0;
    glTranslatef(0, 10, 0);
    glRotatef(theta, 0, 1, 0);
    glRotatef(theta/2, 1, 0, 0);
    glRotatef(theta/3, 0, 0, 1);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(-45, 1, 0, 0);
    float headlight_pos[] = {0, 4, 0, 1};
    float spotdir[] = {0, -1, 0};
    for (int j = 0; j < 36; j++){
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < N; i++){
            wx[i] = vx[i]*cos(angStep) + vz[i]*sin(angStep);
            wy[i] = vy[i];
            wz[i] = -vx[i] * sin(angStep) + vz[i]*cos(angStep);
            if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
                            vx[i-1], vy[i-1], vz[i-1],
                            vx[i], vy[i], vz[i]);
            glTexCoord2f(float(j)/36.0, float(i)/N); glVertex3f(vx[i], vy[i], vz[i]);
            if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
                            vx[i], vy[i], vz[i],
                            wx[i], wy[i], wz[i]);
            glTexCoord2f(float((j+1))/36.0, float(i)/N); glVertex3f(wx[i], wy[i], wz[i]);
        }
    glEnd();
        for (int i = 0; i < N; i++){
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }
    glLightfv(GL_LIGHT1, GL_POSITION, headlight_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
    glPopMatrix();

    glPushMatrix();
        glRotatef(theta, 0, 1, 0);
        staff();
    glPopMatrix();
    glPushMatrix();
        glScalef(0.3, 0.3, 0.3);
        glTranslatef(0, 35, 0);
        solarSystem();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(eye_x, 0, eye_z);
        glRotatef(-2*angle*180/(2*M_PI), 0, 1, 0);
        glTranslatef(0, 0, shift);
        characterModel();
    glPopMatrix();

   glutSwapBuffers();   //Useful for animation
   glFlush();
}

void mainTimer(int value)
{
    theta += 1;

    if (wing >= 20) {
        flag = -1;
    }
    else if (wing <= -20) {
        flag = 1;
    }
    wing += flag;
    glutPostRedisplay();
    glutTimerFunc(25, mainTimer, 0);
}

void waspTimer(int value)
{
    wasp_theta += 1.5;

    if (wasp_wing > 30) {
        wasp_flag = -1;
    }
    else if (wasp_wing < -30) {
        wasp_flag = 1;
    }
    wasp_wing += 10*wasp_flag;
    glutPostRedisplay();
    glutTimerFunc(1, waspTimer, 0);
}

void special(int key, int x, int y)
{

    if(key == GLUT_KEY_LEFT) {
        angle -= 0.05;
        charRotate += 5.714;  //Change direction
    }
    else if(key == GLUT_KEY_F1){
        if (camFlag == 1){
            camFlag = 0;
        } else {
            camFlag = 1;
        }
    }
    else if(key == GLUT_KEY_F2){
        multiplier += 1;
    }
    else if(key == GLUT_KEY_F3){
        multiplier -= 1;
    }
    else if(key == GLUT_KEY_RIGHT) {
        angle += 0.05;
        charRotate -= 5.714;
    }
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 0.5*sin(angle);
        eye_z += 0.5*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 0.5*sin(angle);
        eye_z -= 0.5*cos(angle);
    }
    if (camFlag == 0) {
        look_x = eye_x + 100*sin(angle);
        look_z = eye_z - 100*cos(angle);
        look_y = 0;
        eye_y = 15;
        shift = -7;
    } else {
        look_x = eye_x + 75*sin(angle);
        look_z = eye_z - 75*cos(angle);
        look_y = -35;
        eye_y = 25;
        shift = -7;
    }
    glutPostRedisplay();
}


//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("COSC361 Assignment 1 Created by Thomas Bingham");
   initialize ();
   glutTimerFunc(25, mainTimer, 0);
   glutTimerFunc(1, waspTimer, 0);
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
