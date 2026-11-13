#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>

#define width 400
#define height 600
#define PI 3.1415
#define polygon_num 10

int mouse_motion = 0;
int target_on = 0;

float left = 0.0;
float bottom = 0.0;

float view_width = width;
float view_height = height;

float radius = 20.0;
float point[1000][2];
int point_num = 0;

float target_left = 100.0;
float target_bottom = 100.0;
float target_width = 100.0;
float target_height = 100.0;

typedef struct _Point {
    float x;
    float y;
} Point;

Point circle_center;

void Modeling_Axis(void) {
    glLineWidth(2.0);
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(0, 0);
    glVertex2f(view_width, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, view_height);

    glEnd();
}

void Modeling_Circle(Point CC) {
    float delta;
    float x, y;

    glColor3f(0.0, 1.0, 1.0);
    glPointSize(3.0);

    delta = 2 * PI / polygon_num;

    glBegin(GL_POLYGON);
    for (int i = 0; i < polygon_num; i++) {
        x = CC.x + radius * cos(delta * i);
        y = CC.y + radius * sin(delta * i);
        glVertex2f(x, y);
    }
    glEnd();
}

void Modeling_Points_from_Screen(void) {
    glPointSize(5.0);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_POINTS);
    for (int k = 0; k < point_num; k++)
        glVertex2f(point[k][0], point[k][1]);
    glEnd();
}

void Target_Rectangle(void) {
    if (target_on)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex2f(target_left, target_bottom);
    glVertex2f(target_left + target_width, target_bottom);
    glVertex2f(target_left + target_width, target_bottom + target_height);
    glVertex2f(target_left, target_bottom + target_height);
    glEnd();
}

void RenderScene(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, left + view_width, bottom, bottom + view_height);

    Modeling_Axis();
    Target_Rectangle();
    Modeling_Circle(circle_center);
    Modeling_Points_from_Screen();

    glFlush();
}

void mouse2(int button, int state, int x, int y) {
    float cx, cy;
    float scale;

    cx = left + (float)x / width * view_width;
    cy = bottom + (float)(height - y) / height * view_height;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        if ((target_left <= cx) && (cx <= target_left + target_width) &&
            (target_bottom <= cy) && (cy <= target_bottom + target_height)) {

            target_on = !target_on;
            glutPostRedisplay();
            return;
        }

        point[point_num][0] = cx;
        point[point_num][1] = cy;
        point_num++;

        circle_center.x = cx;
        circle_center.y = cy;

        scale = 0.8;

        left = cx - (cx - left) * scale;
        bottom = cy - (cy - bottom) * scale;

        view_width *= scale;
        view_height *= scale;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        if ((target_left <= cx) && (cx <= target_left + target_width) &&
            (target_bottom <= cy) && (cy <= target_bottom + target_height)) {

            target_on = !target_on;
            glutPostRedisplay();
            return;
        }

        scale = 1.25;

        left = cx - (cx - left) * scale;
        bottom = cy - (cy - bottom) * scale;

        view_width *= scale;
        view_height *= scale;
    }

    glutPostRedisplay();
}

void motion(int x, int y) {
    circle_center.x = left + (float)x / width * view_width;
    circle_center.y = bottom + (float)(height - y) / height * view_height;
    glutPostRedisplay();
}

void MySpecial(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT: left += view_width * 0.05; break;
    case GLUT_KEY_RIGHT: left -= view_width * 0.05; break;
    case GLUT_KEY_DOWN: bottom += view_height * 0.05; break;
    case GLUT_KEY_UP: bottom -= view_height * 0.05; break;
    default: break;
    }
    glutPostRedisplay();
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Final Version");
    glutDisplayFunc(RenderScene);
    glutMouseFunc(mouse2);
    glutMotionFunc(motion);
    glutSpecialFunc(MySpecial);
    glutMainLoop();
}