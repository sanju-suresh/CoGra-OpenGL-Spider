#include<GLES3/gl3.h>
#include <GL/glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
double windowHeight=1080, windowWidth=720;
double eyeX=4 , eyeY=3.0, eyeZ=11.5, refX = -5.5, refY=-11.5,refZ=-4;

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
void newBed()
{
    glPushMatrix();
    glTranslatef(0,0,0.7);
    
    // Bed frame
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(0.8, 0.3, 1.2); // Adjust scale as needed
    drawCube1(0.5, 0.7, 0.05, 0.25, 0.175, 0.025); // Adjust colors and dimensions
    glPopMatrix();
    
     glPushMatrix();
    glTranslatef(0.1,0.5,0.1);
    // Mattress
    glPushMatrix();
    glTranslatef(0, 0.3, 0);
    glScalef(0.75, 0.1, 1.1); // Adjust scale as needed
    drawCube1(0.8, 0.8, 0.8, 0.4, 0.02, 0.35); // Adjust colors and dimensions
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
    std::cout<<eyeX<<" "<<eyeY<<" "<<eyeZ<<std::endl;
    std::cout<<refX<<" "<<refY<<" "<<refZ<<std::endl;
    cout<<"------"<<endl;
    
    switch ( key )
    {
        case 'w': // move eye point upwards along Y axis
            eyeY+=0.5;
            break;
        case 's': // move eye point downwards along Y axis
            eyeY-=0.5;
            break;
        case 'a': // move eye point left along X axis
            eyeX-=0.5;
            break;
        case 'd': // move eye point right along X axis
            eyeX+=0.5;
            break;
        case 'o':  //zoom out
            eyeZ+=0.5;
            break;
        case 'i': //zoom in
            eyeZ-=0.5;
            break;
        case 'q': //back to default eye point and ref point
                eyeX=7.0; eyeY=2.0; eyeZ=15.0;
                refX=0.0; refY=0.0; refZ=0.0;
            break;
        case 'j': // move ref point upwards along Y axis
            refY+=0.5;
            break;
        case 'n': // move ref point downwards along Y axis
            refY-=0.5;
            break;
        case 'b': // move ref point left along X axis
            refX-=0.5;
            break;
        case 'm': // move eye point right along X axis
            refX+=0.5;
            break;
        case 'k':  //move ref point away from screen/ along z axis
            refZ+=0.5;
            break;
        case 'l': //move ref point towards screen/ along z axis
            refZ-=0.5;
            break;
       
        case 27:    // Escape key
            exit(1);
    }
    
    glutPostRedisplay();
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
     drawCube1(1.0, 0.9, 0.8,  0.5,0.45,0.4);
     glPopMatrix();
    
    // floor
    glPushMatrix();
    glScalef(5, 0.1, 7);
    glTranslatef(-1,-5,0); //-1,-5,.5
    //glScalef(5, 0.1, 7);
    drawCube1(0.5, 0.1, 0.0,  0.25,0.05,0);
    glPopMatrix();
}

void display(void){
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60,1,1,100);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ,  refX,refY,refZ,  0,1,0);

    glEnable(GL_LIGHTING);
    glEnable( GL_LIGHT0);
    GLfloat light_position[] = { 5.0f, 5.0f, 15.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    room();
    newBed();
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

    // glutReshapeFunc(fullScreen);
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);
    // glutIdleFunc(animate);
    glutMainLoop();

    

    return 0;
}