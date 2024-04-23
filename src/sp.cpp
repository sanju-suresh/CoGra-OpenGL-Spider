#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

double windowHeight = 1080, windowWidth = 720;
double eyeX = 4, eyeY = 3.0, eyeZ = 11.5, refX = -5.5, refY = -11.5, refZ = -4;
float spiderX = 0.0f;
float spiderZ = 0.0f;
bool spiderForward = true;
float spiderSpeed = 0.05f;

void drawSpider() {
    glPushMatrix();
    glTranslatef(3.0f + spiderX, 1.5f, 10.0f + spiderZ);
    glScalef(0.1f, 0.1f, 0.1f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidSphere(0.5, 20, 20);

    // Leg segments
    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = 0; i < 8; ++i) {
        glPushMatrix();
        float angle = (float)i * 45.0f;
        if (i < 4)
            glTranslatef(0.3f, 0.0f, 0.0f);
        else
            glTranslatef(-0.3f, 0.0f, 0.0f);

        // First segment
        glPushMatrix();
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glRotatef(-30.0f, 1.0f, 0.0f, 1.0f);
        glutSolidSphere(0.1, 10, 10); // Joint sphere

        // Hinge
        glTranslatef(0.0f, 0.0f, 0.1f);
        glutSolidSphere(0.1, 10, 10); // Hinge sphere

        // Second segment
        glTranslatef(0.0f, 0.0f, 0.1f); // Move to the second segment start position
        glRotatef(45.0f, 1.0f, 0.0f, 1.0f); // Apply hinge rotation
        glutSolidSphere(0.1, 10, 10); // Joint sphere
        glPopMatrix();
        glPopMatrix();
    }

    // Eyes
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.2f, 0.2f, 0.3f);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f, 0.2f, 0.3f);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void animateSpider(int value) {
    // Update spider position based on direction
    if (spiderForward) {
        spiderZ -= spiderSpeed;
        if (spiderZ <= -3.0f)
            spiderForward = false;
    } else {
        spiderZ += spiderSpeed;
        if (spiderZ >= 3.0f)
            spiderForward = true;
    }

    // Redraw the scene
    glutPostRedisplay();

    // Call the function again after a certain interval
    glutTimerFunc(30, animateSpider, 0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, refX, refY, refZ, 0, 1, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {5.0f, 5.0f, 15.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Draw the scene objects
    drawSpider();

    glDisable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Spider");

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glutDisplayFunc(display);
    glutTimerFunc(0, animateSpider, 0);

    glutMainLoop();

    return 0;
}
