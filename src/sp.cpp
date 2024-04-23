#include <GLES3/gl3.h>
#include <GL/glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
bool isShiftPressed = false;

void myKeyboardFunc(unsigned char key, int x, int y) {
    if (key == 27) // ESC key
        exit(0);

    if (key == GLUT_KEY_SHIFT_L || key == GLUT_KEY_SHIFT_R)
        isShiftPressed = true;
    else
        isShiftPressed = false;
}

void mySpecialKeyboardFunc(int key, int x, int y) {
    float val = 0.03f;

    if (isShiftPressed) {
        switch (key) {
            case GLUT_KEY_LEFT:
                theta -= val;
                break;
            case GLUT_KEY_RIGHT:
                theta += val;
                break;
        }
    } else {
        switch (key) {
            case GLUT_KEY_UP:
                spiderZ -= val;
                break;
            case GLUT_KEY_DOWN:
                spiderZ += val;
                break;
            case GLUT_KEY_LEFT:
                spiderX -= val;
                break;
            case GLUT_KEY_RIGHT:
                spiderX += val;
                break;
        }
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Spider Animation");

    glutDisplayFunc(display);

    glutSpecialFunc(mySpecialKeyboardFunc);
    glutKeyboardFunc(myKeyboardFunc);

    glutMainLoop();
    return 0;
}
