#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLES3/gl3.h>
#include <GL/glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
using namespace std;
double windowHeight=1080, windowWidth=720;
double eyeX=4 , eyeY=3.0, eyeZ=11.5, refX = -5.5, refY=-11.5,refZ=-4;
float spiderX = 0.0f;
float spiderY =0.0f;
float spiderZ = 0.0f;
bool spiderForward = true;
float spiderSpeed = 0.05f;
bool amb1=true, diff1=true, spec1=true;
bool isKeyHeld =false;
bool isShiftPressed=false;
GLuint tableTexture;
bool switchOne = true;

float legAngle = 0.0f; // Initial angle for leg movement
float legDirection = 1.0f; 

float theta=180.0f;


bool fwdExtend=false;
bool bckExtend=false;



static GLfloat v_cube[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 3.0}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.0, 3.0, 0.0}, //4
    {0.0, 3.0, 3.0}, //5
    {3.0, 3.0, 3.0}, //6
    {3.0, 3.0, 0.0}  //7
};

static GLubyte quadIndices[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {0, 4, 7, 3}, // back is clockwise
    {2, 3, 7, 6}, //right
    {1, 5, 4, 0}  //left is clockwise
};

static void getNormal3p (GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void drawCylinder(float radius, float height, int slices, int stacks) {
    GLfloat mat_shininess[] = {100};
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    GLUquadricObj *obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluCylinder(obj, radius, radius, height, slices, stacks);
    gluDeleteQuadric(obj);
}


void drawSphere(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=90)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    
    glutSolidSphere (3.0, 20, 16);
}

void myOwnSphere(GLdouble ac1, GLint ac2, GLint ac3, GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=90)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    
    glutSolidSphere (ac1, ac2, ac3);
}

void drawCube1(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX=0, GLfloat ambY=0, GLfloat ambZ=0, GLfloat shine=50)
{
   
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    glBegin(GL_QUADS);
    
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
                    v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
                    v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }
    glEnd();

    
}
void lightBulb1()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1.000, 1,1, 0.0};
    
    glPushMatrix();
    glTranslatef (5, 5, 8);
    glScalef(0.2, 0.2, 0.2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    if(switchOne == true){
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }else{
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();
}


void centerTable()
{
    glPushMatrix();
    glTranslatef(3, 0, 10);
    
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tableTexture); // Replace textureID with the ID of your texture
    
    // Table top
    glPushMatrix();
    glTranslatef(-0.7, 1.2, -1);
    glScalef(0.6, 0.05, 0.6);
    
    // Specify texture coordinates
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0, 3.3, 0);
    glTexCoord2f(1.0, 0.0); glVertex3f(3, 3.3, 0);
    glTexCoord2f(1.0, 1.0); glVertex3f(3, 3.3, 3);
    glTexCoord2f(0.0, 1.0); glVertex3f(0, 3.3, 3);
    glEnd();
    
    drawCube1(0.7, 0.55, 0.25, 0.25, 0.025, 0.25); 
    
    glPopMatrix();
    
    // Disable texture mapping
    glDisable(GL_TEXTURE_2D);
    
    // Table legs
    float legHeight = 0.35;
    float legThickness = 0.05;
    
    
    // Front left leg
    glPushMatrix();
    glTranslatef(-0.4, 0.2, -0.4); // Adjust position as needed
    glScalef(legThickness, legHeight, legThickness); // Adjust scale as needed
    drawCube1(0.5, 0.35, 0.05, 0.25, 0.025, 0.25); // Adjust colors and dimensions
    glPopMatrix();
    
    // Front right leg
    glPushMatrix();
    glTranslatef(0.6, 0.2, -0.4); // Adjust position as needed
    glScalef(legThickness, legHeight, legThickness); // Adjust scale as needed
    drawCube1(0.5, 0.35, 0.05, 0.25, 0.025, 0.25); // Adjust colors and dimensions
    glPopMatrix();
    
    // Back left leg
    glPushMatrix();
    glTranslatef(-0.4, 0.2, 0.4); // Adjust position as needed
    glScalef(legThickness, legHeight, legThickness); // Adjust scale as needed
    drawCube1(0.5, 0.35, 0.05, 0.25, 0.025, 0.25); // Adjust colors and dimensions
    glPopMatrix();
    
    // Back right leg
    glPushMatrix();
    glTranslatef(0.6, 0.2, 0.4); // Adjust position as needed
    glScalef(legThickness, legHeight, legThickness); // Adjust scale as needed
    drawCube1(0.5, 0.35, 0.05, 0.25, 0.025, 0.25); // Adjust colors and dimensions
    glPopMatrix();

    glPopMatrix();
}

void drawSpider() {
    

    glPushMatrix();
    
    glTranslatef(3.0f + spiderX, 1.5f + spiderY, 10.0f + spiderZ); 
    glScalef(0.1f, 0.1f, 0.1f);
    glRotatef(180+theta, 0.0f, 1.0f, 0.0f);

    if(spiderY){
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    }
    if(spiderY>=3.3f){
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    }
    // glColor3f(0.8f, 0.0f, 1.0f); // Body color
    // glutSolidSphere(0.5, 20, 20);
    myOwnSphere(0.5, 20, 20, 0.0f, 0.0f, 0.3f, 0.25f, 0.25f, 0.25f );

    glPushMatrix();
    glTranslatef(0.1,0.0 , -1.0);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    glRotatef(10, 0.0f, 1.0f, 0.0f);

    // Leg segments
    glColor3f(0.5f, 1.0f, 0.9f); // Leg color
    for (int i = 0; i < 8; ++i) {
        glPushMatrix();
        legDirection*=-1;
        
        float angle = (float)i * 45.0f;
        if (i < 4)
        {
    
            glTranslatef(0.3f, 0.0f, 0.0f);
        }
        else
        {
            glTranslatef(-0.3f, 0.0f, 0.0f);
        }
        glRotatef(angle , 0.0f, 1.0f, 0.0f);
        glRotatef(-30.0f , 1.0f, 0.0f, 1.0f); // Apply leg animation
        if(i%2==0)
            glRotatef(0.35*legAngle, 0.0f, 1.0f, 0.0f);
        else 
            glRotatef(-0.35*legAngle, 0.0f, 1.0f, 0.0f);

        if (fwdExtend ) {
            if(i==0)
                glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);

            if(i==7) 
                glRotatef(+30.0f, 0.0f, 1.0f, 0.0f);
            
            glRotatef(15.0f, 1.0f, 0.0f, 1.0f);
        }
        if (bckExtend ) {
            if(i==3)
                glRotatef(+30.0f, 0.0f, 1.0f, 0.0f);

            if(i==4) 
                glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
            
            glRotatef(15.0f, 1.0f, 0.0f, 1.0f);
        }
        drawCylinder(0.1, 1.5, 10, 10);

        // Hinge
        glTranslatef(0.0f, 0.0f, 1.5f);
        glutSolidSphere(0.1, 10, 10); // Hinge sphere

        // Second segment
        glTranslatef(0.0f, 0.0f, 0.0f); // Move to the second segment start position
        glRotatef(45.0f , 1.0f, 0.0f, 0.0f); // Apply leg animation
        if(fwdExtend){
            if(i==0 || i==7) glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);

        }
        if(bckExtend){
            if(i==3 || i==4) glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);

        }
        glRotatef(45.0f + legAngle*0.75 , 0.85f, 0.1f, 0.85f);
        

        // cout<<legAngle<<endl;
        drawCylinder(0.1, 1.5, 10, 10);

        
        glPopMatrix();


            }
    // glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(-0.10f, 0.2f, 0.3f);
    myOwnSphere(0.15, 10, 10, 0.9f, 0.9f, 0.9f, 0.25f, 0.25f, 0.25f);
    // glScalef(0.07f, 0.07f, 0.07f);
    // drawCube1(0.7f, 0.0f, 0.0f ,  0.5f, 0.5f, 0.5f ,100);
    glPopMatrix();

    // glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.10f, 0.2f, 0.3f);
    myOwnSphere(0.15, 10, 10, 0.9f, 0.9f, 0.9f, 0.25f, 0.25f, 0.25f);
    // glScalef(0.07f, 0.07f, 0.07f);
    // drawCube1(0.7f, 0.0f, 0.0f , 0.5f, 0.5f, 0.5f ,100);
    glPopMatrix();

    glPopMatrix();
}


void lightOne()
{
    glPushMatrix();
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 0.5f}; 
    GLfloat light_specular[] ={0.25f, 0.25f, 0.25f, 1.0f}; 
    // GLfloat light_specular[] ={0 ,0,0,0}; 
    // GLfloat light_position[] = { 5.0, 5.0, 8.0, 1.0 }; 
    GLfloat light_position[] = { 7.0, 5.0, 7.0, 1.0 }; 
    
    //glEnable( GL_LIGHT0);
    
    if(amb1 == true){glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);}
    
    if(diff1 == true){glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);}
    
    if(spec1 == true){glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);}
   
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

    
    // Set light position
    // GLfloat light_position[] = { 5.0f, 5.0f, 15.0f, 1.0f }; 
    // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glPopMatrix();
}




void webThread(){
    if(!spiderY || spiderY==3.5) return;
    glPushMatrix();


    
    glTranslatef(3.0f + spiderX, 1.5f + spiderY, 10.0f + spiderZ); 
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.005, 4, 10, 10);

    
    
    glPopMatrix();


}

void animateSpiderLegs(int v) {
    if (isKeyHeld) {
        // Update leg angle
        legAngle -= 1.5f * legDirection;

        // Reverse leg direction when reaching limits
        //  if (legAngle > 5.0f || legAngle < -5.0f) {
        if (legAngle > 15.0f || legAngle < -15.0f) {
            legDirection *= -1.0f;
        }
        glutPostRedisplay();
    }

    glutTimerFunc(5, animateSpiderLegs, 0);
}

void polygon(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(6,0);
    glVertex2f(5.8,1);
    glVertex2f(5.2,2);
    glVertex2f(5, 2.2);
    glVertex2f(4, 2.8);
    glVertex2f(3,3);
    glVertex2f(2, 2.8);
    glVertex2f(1, 2.2);
    glVertex2f(0.8, 2);
    glVertex2f(0.2,1);
    //glVertex2f(0,0);
    
    glEnd();
}

void polygonLine(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_LINE_STRIP);
    //glVertex2f(0,0);
    glVertex2f(6,0);
    glVertex2f(5.8,1);
    glVertex2f(5.2,2);
    glVertex2f(5, 2.2);
    glVertex2f(4, 2.8);
    glVertex2f(3,3);
    glVertex2f(2, 2.8);
    glVertex2f(1, 2.2);
    glVertex2f(0.8, 2);
    glVertex2f(0.2,1);
    glVertex2f(0,0);
    
    glEnd();
}

void dressingTable()
{
    //Dressing table ************************************************
        glPushMatrix();
        glTranslatef(-3, 0, -1);

        //dressing table left body
        glPushMatrix();
        
        glTranslatef(5.9,0,4.6);
        glScalef(0.2, 0.2, 0.2);
        drawCube1(0.545, 0.271, 0.075,  0.2725,0.1355,0.0375);
        glPopMatrix();

        
        //dressing table right body
        glPushMatrix();
        glTranslatef(7,0,4.6);
        glScalef(0.2, 0.2, 0.2);
        drawCube1(0.545, 0.271, 0.075,  0.2725,0.1355,0.0375);
        glPopMatrix();

        
        //dressing table upper body
        glPushMatrix();
        glTranslatef(5.9,0.6,4.6);
        glScalef(0.57, 0.1, 0.2);
        drawCube1(0.545, 0.271, 0.075,  0.2725,0.1355,0.0375);
        glPopMatrix();
        
        //dressing table upper body bottom stripe
        glPushMatrix();
        glTranslatef(5.9,0.6,5.2);
        //glRotatef(22, 0,0,1);
        glScalef(0.57, 0.01, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table upper body upper stripe
        glPushMatrix();
        glTranslatef(5.9,0.9,5.2);
        //glRotatef(22, 0,0,1);
        glScalef(0.57, 0.01, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table upper body handle
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(6.5,0.75,5.2);
        //glRotatef(22, 0,0,1);
        glScalef(0.16, 0.02, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table left body handle
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(6.4,0.1,5.2);
        //glRotatef(22, 0,0,1);
        glScalef(0.02, 0.13, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table right body handle
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(7.1,0.1,5.2);
        //glRotatef(22, 0,0,1);
        glScalef(0.02, 0.13, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table main mirror
        glPushMatrix();
        glTranslatef(6.2,0.9,4.7);
        //glRotatef(22, 0,0,1);
        glScalef(0.36, 0.5, 0.0001);
        drawCube1(0.690, 0.878, 0.902,  0.345,0.439,0.451, 10);
        glPopMatrix();
        
        //dressing table left mirror
        glPushMatrix();
        glTranslatef(5.92,0.9,4.7);
        //glRotatef(0, 0,1,0);
        glScalef(0.1, 0.48, 0.0001);
        drawCube1(0.690, 0.878, 0.902,  0.345,0.439,0.451, 10);
        glPopMatrix();
        
        //dressing table left mirror left stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(5.92,0.9,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.019, 0.48, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table left mirror left stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(6.17,0.9,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.019, 0.48, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table mirror  stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(5.92,0.9,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.55, 0.019, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table left mirror upper stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(5.92,2.3,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.1, 0.019, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table right mirror
        glPushMatrix();
        glTranslatef(7.25,0.9,4.7);
        //glRotatef(-40, 0,1,0);
        glScalef(0.1, 0.48, 0.0001);
        drawCube1(0.690, 0.878, 0.902,  0.345,0.439,0.451, 10);
        glPopMatrix();
        
        //dressing table left mirror upper stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(7.25,2.3,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.1, 0.019, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table right mirror left stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(7.25,0.9,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.019, 0.48, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();
        
        //dressing table right mirror right stripe
        glColor3f(0.2,0.1,0.1);
        glPushMatrix();
        glTranslatef(7.5,0.9,4.71);
        //glRotatef(22, 0,0,1);
        glScalef(0.019, 0.48, 0.0001);
        drawCube1(0.2,0.1,0.1,  0.1,0.05,0.05);
        glPopMatrix();

        glPopMatrix();
}

void newBed()
{
    glPushMatrix();
    glTranslatef(0,0,0.7);
    
    // Bed frame
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(0.8, 0.3, 1.2); // Adjust scale as needed
    drawCube1(0.5, 0.2, 0.0, 0.25, 0.25, 0.025); // Adjust colors and dimensions
    glPopMatrix();
    
     glPushMatrix();
    glTranslatef(0.1,0.5,0.1);
    // Mattress
    glPushMatrix();
    glTranslatef(0, 0.3, 0);
    glScalef(0.75, 0.1, 1.1); // Adjust scale as needed
    drawCube1(0.6, 0.8, 1.0, 0.35, 0.35, 0.35); // Adjust colors and dimensions
    glPopMatrix();
    
    // Pillow 1
    glPushMatrix();
    glTranslatef(0.3, 0.7, 0.4); // Adjust position as needed
    glScalef(0.27, 0.08, 0.2); // Adjust scale as needed
    drawCube1(1.0, 1.0, 1.0, 0.2, 0.1, 0.3); // Adjust colors and dimensions
    glPopMatrix();
    
    // Pillow 2
    glPushMatrix();
    glTranslatef(1.2, 0.7, 0.4); // Adjust position as needed
    glScalef(0.27, 0.08, 0.2); // Adjust scale as needed
    drawCube1(1.0, 1.0, 1.0, 0.2, 0.1, 0.3); // Adjust colors and dimensions
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}



void myKeyboardFunc( unsigned char key, int x, int y )
{
    float val=0.3f;
    float verticalDisp = 0.05f;
    switch ( key )
    {
        
        case 'f': // move eye point upwards along Y axis
            eyeY+=val;
            break;
        case 'v': // move eye point downwards along Y axis
            eyeY-=val;
            break;
        case 'c': // move eye point left along X axis
            eyeX-=val;
            break;
        case 'b': // move eye point right along X axis
            eyeX+=val;
            break;
        case 'o':  //zoom out
            eyeZ+=1;
            break;
        case 'i': //zoom in
            eyeZ-=1;
            break;
        case 'q': //back to default eye point and ref point
                eyeX=7.0; eyeY=2.0; eyeZ=15.0;
                refX=0.0; refY=0.0; refZ=0.0;
            break;
        case 'j': // move ref point upwards along Y axis
            refY+=val;
            break;
        case 'm': // move ref point downwards along Y axis
            refY-=val;
            break;
        case 'n': // move ref point left along X axis
            refX-=val;
            break;
        case ',': // move eye point right along X axis
            refX+=val;
            break;
        // case 'k':  //move ref point away from screen/ along z axis
        //     refZ+=1;
        //     break;
        // case 'l': //move ref point towards screen/ along z axis
        //     refZ-=1;
        //     break;

        case 'h': //move spider up
            spiderY+=verticalDisp;
            spiderY=min(spiderY,3.5f);
            break;
        case 'l': //move spider up
            spiderY-=verticalDisp;
            spiderY=max(spiderY,0.0f);
            break;
        case 'd': //Rotate spider right
            theta+=90;
            break;
        case 'a': //Rotate spider left
            theta-=90;
            break;
        case 'w': //Extend leg forward
            fwdExtend=true;
            break;
        case 's': //Extend leg backward
            bckExtend=true;
            break;
        case '1': //to turn on and off light one
            if(switchOne == false)
            {
                switchOne = true; amb1=true; diff1=true; spec1=true;
                glEnable( GL_LIGHT0); break;
            }
            else if(switchOne == true)
            {
                switchOne = false; amb1=false; diff1=false; spec1=false; glDisable( GL_LIGHT0); break;
            }

        
        
        case 27:    // Escape key
            exit(1);
    }
    
    glutPostRedisplay();
}

void mySpecialKeyboardFunc(int key, int x, int y) {
    float val = 0.03f;
    // Check if the Shift key is pressed along with the arrow key
    isShiftPressed = glutGetModifiers() && GLUT_ACTIVE_SHIFT;

    // If Shift is pressed along with the arrow key
    if (isShiftPressed) {
        int rotV=1.0f;
        switch (key) {
            
            case GLUT_KEY_LEFT:
                cout<<theta<<endl;
                theta+=rotV;
                isKeyHeld = true;
                break;
            case GLUT_KEY_RIGHT:
                cout<<theta<<endl;
                theta-=rotV;
                isKeyHeld = true;
                // Do something for Shift + Right arrow key
                break;
        }
    } else {
        // Handle normal arrow key press without Shift
        switch (key) {
            case GLUT_KEY_UP:
                spiderZ -= (val*cos(theta* (M_PI / 180.0)))*(spiderY==3.5f? -1: 1);
                spiderX -= val*sin(theta* (M_PI / 180.0) );
                isKeyHeld = true; // Move the spider backward
                break;
            case GLUT_KEY_DOWN:
                spiderZ += (val*cos(theta* (M_PI / 180.0)))*(spiderY==3.5f? -1: 1);
                spiderX += val*sin(theta* (M_PI / 180.0) );
                isKeyHeld = true; // Move the spider forward
                break;
            case GLUT_KEY_LEFT:
                spiderX -= val*cos(theta* (M_PI / 180.0) );
                spiderZ += val*sin(theta* (M_PI / 180.0) );
                isKeyHeld = true; // Move the spider left
                break;  
            case GLUT_KEY_RIGHT:
                spiderX += val*cos(theta* (M_PI / 180.0) );
                spiderZ -= val*sin(theta* (M_PI / 180.0) );
                isKeyHeld = true; // Move the spider right
                break;
        }
        
    }

    glutPostRedisplay();
}

void mySpecialKeyboardUpFunc(int key, int x, int y) {
    isKeyHeld = false;
    isShiftPressed=false;
    fwdExtend=false;
    bckExtend=false;
}

GLuint loadTexture(const char *filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Load image data
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    return textureID;
}


void cupboard()
{
    //Cupboard/Almari ************************************************************
        glPushMatrix();
        glTranslatef(-8,-0.25,-3);
        //cupboard
    glPushMatrix();
    glTranslatef(4,0,4.4);
    glScalef(0.5, 1, 0.5);
    drawCube1(0.5,0.2,0.2,  0.25, 0.1, 0.1 );
    glPopMatrix();
        
    //cupboard's 1st vertical stripline
    glPushMatrix();
    glTranslatef(4,1,5.9);
    glScalef(0.5, 0.01, 0.0001);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
        
    //cupboard's 2nd vertical stripline
    glPushMatrix();
    glTranslatef(4,0.5,5.9);
    glScalef(0.5, 0.01, 0.0001);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
        
    //cupboard's last stripline
    glPushMatrix();
    glTranslatef(4,0,5.9);
    glScalef(0.5, 0.01, 0.0001);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
        
    //cupboard's lst horizontal stripline
    glPushMatrix();
    glTranslatef(5.5,0,5.9);
    glScalef(0.01, 1, 0.0001);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
        
    //cupboard's right side horizontal stripline
    glPushMatrix();
    glTranslatef(4.75,1,5.9);
    glScalef(0.01, 0.67, 0.0001);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
    
    //cupboard's left side horizontal stripline
    glPushMatrix();
    glTranslatef(4,0,5.9);
    glScalef(0.01, 1, 0.0001);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
        
    //cupboard's handle right
    glPushMatrix();
    glTranslatef(5,1.4,5.9);
    glScalef(0.02, 0.18, 0.01);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
    
    //cupboard's handle right sphere
    glPushMatrix();
    glTranslatef(5.02,1.9,5.91);
    glScalef(0.02, 0.02, 0.01);
    drawSphere(0.2,0.1,0.1,  0.1, 0.05, 0.05,  10);
    glPopMatrix();
    
    //cupboard's handle left
    glPushMatrix();
    glTranslatef(4.5,1.4,5.9);
    glScalef(0.02, 0.18, 0.01);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
    
    //cupboard's handle left sphere
    glPushMatrix();
    glTranslatef(4.52,1.9,5.91);
    glScalef(0.02, 0.02, 0.01);
    drawSphere(0.2,0.1,0.1,  0.1, 0.05, 0.05,  10);
    glPopMatrix();
        
    //cupboard's drawer's 1st handle
    glPushMatrix();
    glTranslatef(4.5,0.7,5.9);
    glScalef(0.16, 0.02, 0.01);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();
        
    //cupboard's drawer's 2nd handle
    glPushMatrix();
    glTranslatef(4.5,0.25,5.9);
    glScalef(0.16, 0.02, 0.01);
    drawCube1(0.2,0.1,0.1,  0.1, 0.05, 0.05);
    glPopMatrix();

    glPopMatrix();
}

void room()
{
   
    // right wall
    //glColor3f(1, 0.8, 0.5);
    glPushMatrix();
    glTranslatef(-5,-1,.5);
    glScalef(5, 2, 0.1);
    //drawCube1(1, 0.8, 0.5,  0.5,0.4,0.25);
    drawCube1(1, 0.8, 0.7,  0.5, 0.4, 0.35);
    glPopMatrix();
    
    // left wall
    //glColor3f(1, 0.8, 0.7);
    glPushMatrix();
    glTranslatef(-4.5,-1,0);
    glScalef(0.2, 2, 5);
    drawCube1(1, 0.8, 0.7,  0.5, 0.4, 0.35);
    glPopMatrix();
    
    // wall besides the right wall
    //glColor3f(1, 0.8, 0.7);
    glPushMatrix();
    glTranslatef(8,-1,0);
    glScalef(0.2, 2, 5);
    drawCube1(1, 0.8, 0.7,  0.5, 0.4, 0.35);
    glPopMatrix();
    
    //ceiling
     //glColor3f(1.0, 0.9, 0.8);
     glPushMatrix();
     glTranslatef(-4.5,5.1,0);
     glScalef(5, 0.1, 7);
    //  glColor3f(1.0, 0.9, 0.8);
     drawCube1(0.1, 0.8, 0.9,  0.8,0.8,0.8);
     glPopMatrix();
    
    // floor
    glPushMatrix();
    glScalef(5, 0.1, 7);
    glTranslatef(-1,-5,0); //-1,-5,.5
    //glScalef(5, 0.1, 7);
    drawCube1(0.3, 0.5, 0.6,  0.25,0.25,0.25);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, refX, refY, refZ, 0, 1, 0);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

// // // Set ambient light color
//     GLfloat ambientLight[] = {0.25f, 0.25f, 0.25f, 1.0f}; // Ambient light color (RGBA)
//     glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

//     // Set diffuse light color
//     GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 0.5f}; // Diffuse light color (RGBA)
//     glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

//     // Set specular light color
//     GLfloat specularLight[] = {0.25f, 0.25f, 0.25f, 1.0f}; // Specular light color (RGBA)
//     glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

//     // Set light position
//     GLfloat light_position[] = { 5.0f, 5.0f, 15.0f, 1.0f }; 
//     glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    
    lightOne();
    lightBulb1();
    room();
    newBed();
    dressingTable();
    drawSpider();
    webThread();
    centerTable();
    cupboard();
    glDisable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Spider");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    tableTexture = loadTexture("table.jpg");

    // glutReshapeFunc(fullScreen);
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);

     glutSpecialFunc(mySpecialKeyboardFunc);

     glutSpecialUpFunc(mySpecialKeyboardUpFunc);
    // glutKeyboardFunc(mySpecialKeyboardFunc);
    // glutTimerFunc(0, animateSpider, 0);
    // glutIdleFunc(animate);


    // Register the animation function
    glutTimerFunc(0, animateSpiderLegs, 0);
    glutMainLoop();

    

    return 0;
}