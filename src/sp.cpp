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

bool isKeyHeld =false;
bool isShiftPressed=false;

float legAngle = 0.0f; // Initial angle for leg movement
float legDirection = 1.0f; 

float theta=0.0f;


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


void centerTable()
{
    glPushMatrix();
    glTranslatef(3, 0, 10);
    // Table top
    glPushMatrix();
    glTranslatef(-0.7, 1.2, -1);
    glScalef(0.6, 0.05, 0.6); // Adjust scale as needed
    drawCube1(0.7, 0.55, 0.25, 0.25, 0.025, 0.25); // Adjust colors and dimensions
    glPopMatrix();
    
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
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(-0.05f, 0.2f, 0.3f);
    // glutSolidSphere(0.15, 10, 10);
    glScalef(0.07f, 0.07f, 0.07f);
    drawCube1(0.7f, 0.0f, 0.0f ,  0.5f, 0.5f, 0.5f ,100);
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.30f, 0.2f, 0.3f);
    // glutSolidSphere(0.15, 10, 10);
    glScalef(0.07f, 0.07f, 0.07f);
    drawCube1(0.7f, 0.0f, 0.0f , 0.5f, 0.5f, 0.5f ,100);
    glPopMatrix();

    glPopMatrix();
}



void webThread(){
    if(!spiderY) return;
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glEnd();
    glTranslatef(3.0f + spiderX, 1.5f + spiderY, 10.0f + spiderZ); 
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.005, 4, 10, 10);

    
    
    glPopMatrix();


}

void animateSpiderLegs(int v) {
    if (isKeyHeld) {
        // Update leg angle
        legAngle -= 1.5f * legDirection;
        if (legAngle > 15.0f || legAngle < -15.0f) {
            legDirection *= -1.0f;
        }
        glutPostRedisplay();
    }

    glutTimerFunc(5, animateSpiderLegs, 0);
}
void myKeyboardFunc( unsigned char key, int x, int y )
{
    float val=0.3f;
    float verticalDisp = 0.05f;
    switch ( key )
    {
        
        case 'w': // move eye point upwards along Y axis
            eyeY+=val;
            break;
        case 's': // move eye point downwards along Y axis
            eyeY-=val;
            break;
        case 'a': // move eye point left along X axis
            eyeX-=val;
            break;
        case 'd': // move eye point right along X axis
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
        case 'n': // move ref point downwards along Y axis
            refY-=val;
            break;
        case 'b': // move ref point left along X axis
            refX-=val;
            break;
        case 'm': // move eye point right along X axis
            refX+=val;
            break;
        case 'h': //move spider up
            spiderY+=verticalDisp;
            spiderY=min(spiderY,3.5f);
            break;
        case 'l': //move spider up
            spiderY-=verticalDisp;
            spiderY=max(spiderY,0.0f);
            break;
        case 'z': //Rotate spider right
            theta+=90;
            break;
        case 'x': //Rotate spider left
            theta-=90;
            break;
        case '1': //Extend leg forward
            fwdExtend=true;
            break;
        case '2': //Extend leg backward
            bckExtend=true;
            break;
        
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
                spiderZ -= val*cos(theta* (M_PI / 180.0));
                spiderX -= val*sin(theta* (M_PI / 180.0) );
                isKeyHeld = true; // Move the spider backward
                break;
            case GLUT_KEY_DOWN:
                spiderZ += val*cos(theta* (M_PI / 180.0));
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


void room()
{
    glPushMatrix();
    glTranslatef(-5,-1,.5);
    glScalef(5, 2, 0.1);
    //drawCube1(1, 0.8, 0.5,  0.5,0.4,0.25);
    drawCube1(1, 0.8, 0.7,  0.5, 0.4, 0.35);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-4.5,-1,0);
    glScalef(0.2, 2, 5);
    drawCube1(1, 0.8, 0.7,  0.5, 0.4, 0.35);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(8,-1,0);
    glScalef(0.2, 2, 5);
    drawCube1(1, 0.8, 0.7,  0.5, 0.4, 0.35);
    glPopMatrix();
    
     glPushMatrix();
     glTranslatef(-4.5,5.1,0);
     glScalef(5, 0.1, 7);
    //  glColor3f(1.0, 0.9, 0.8);
     drawCube1(0.1, 0.8, 0.9,  0.5,0.45,0.4);
     glPopMatrix();
    
    // floor
    glPushMatrix();
    glScalef(5, 0.1, 7);
    glTranslatef(-1,-5,0); //-1,-5,.5
    //glScalef(5, 0.1, 7);
    drawCube1(0.5, 0.1, 0.0,  0.25,0.05,0);
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

// // Set ambient light color
    GLfloat ambientLight[] = {0.25f, 0.25f, 0.25f, 1.0f}; // Ambient light color (RGBA)
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // Set diffuse light color
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 0.5f}; // Diffuse light color (RGBA)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    // Set specular light color
    GLfloat specularLight[] = {0.25f, 0.25f, 0.25f, 1.0f}; // Specular light color (RGBA)
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Set light position
    GLfloat light_position[] = { 5.0f, 5.0f, 15.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    room();
    drawSpider();
    webThread();
    centerTable();
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
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);

     glutSpecialFunc(mySpecialKeyboardFunc);

     glutSpecialUpFunc(mySpecialKeyboardUpFunc);
    glutTimerFunc(0, animateSpiderLegs, 0);
    glutMainLoop();
    return 0;
}