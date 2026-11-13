#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

#define PI 3.1415926

int n_gon = 17;
float radius = 1.0f;
double delta_theta;
GLenum model_type = GL_POLYGON;

float angle = 0.0f;
int color_state = 0;
int color_max = 6;

int star_mode = 0;

float c_left = -100.0f;
float c_right = 100.0f;
float c_bottom = -100.0f;
float c_top = 100.0f;

void draw_axis(void) {
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-1000.0, 0.0);
    glVertex2f(1000.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0.0, -1000.0);
    glVertex2f(0.0, 1000.0);

    glEnd();
}

void set_color() {
    switch (color_state) {
    case 0: glColor3f(1.0, 1.0, 0.0); break;
    case 1: glColor3f(1.0, 0.0, 0.0); break;
    case 2: glColor3f(0.0, 1.0, 0.0); break;
    case 3: glColor3f(0.0, 0.0, 1.0); break;
    case 4: glColor3f(1.0, 0.0, 1.0); break;
    case 5: glColor3f(0.0, 1.0, 1.0); break;
    }
}

void draw_Ngon(void) {
    if (model_type == GL_POINTS)
        glPointSize(5.0f);

    set_color();

    delta_theta = 2 * PI / n_gon;

    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(model_type);
    for (int i = 0; i < n_gon; i++) {
        float x = radius * cos(i * delta_theta) * 50;
        float y = radius * sin(i * delta_theta) * 50;
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

void draw_star(void) {
    set_color();

    float R = radius * 50;
    float vx[5], vy[5];

    for (int i = 0; i < 5; i++) {
        float theta = 2 * PI * i / 5 - PI / 2;
        vx[i] = R * cos(theta);
        vy[i] = R * sin(theta);
    }

    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_LINE_LOOP); // 5각 별
    glVertex2f(vx[0], vy[0]);
    glVertex2f(vx[2], vy[2]);
    glVertex2f(vx[4], vy[4]);
    glVertex2f(vx[1], vy[1]);
    glVertex2f(vx[3], vy[3]);
    glEnd();

    glPopMatrix();
}

void RenderScene(void) {
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(c_left, c_right, c_bottom, c_top);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (star_mode)
        draw_star();
    else
        draw_Ngon();

    draw_axis();

    glFlush();
}

void MyKey(unsigned char key, int x, int y) {
    switch (key) {

    case 'a': case 'A': radius += 0.1f; break;
    case 's': case 'S': if (radius > 0.1f) radius -= 0.1f; break;
    case 'n': case 'N': n_gon++; break;
    case 'm': case 'M': if (n_gon > 3) n_gon--; break;
    case 'r': case 'R': angle += 5.0f; break;
    case 'c': case 'C': color_state = (color_state + 1) % color_max; break;
    case 't': case 'T': star_mode = !star_mode; break;
    }

    glutPostRedisplay();
}

void SpecialKey(int key, int x, int y) {
    float pan_speed = 10.0f;

    switch (key) {

    case GLUT_KEY_F1: model_type = GL_POINTS; break;
    case GLUT_KEY_F2: model_type = GL_LINES; break;
    case GLUT_KEY_F3: model_type = GL_LINE_LOOP; break;
    case GLUT_KEY_F4: model_type = GL_POLYGON; break;

    case GLUT_KEY_LEFT: c_left -= pan_speed; c_right -= pan_speed; break;
    case GLUT_KEY_RIGHT: c_left += pan_speed; c_right += pan_speed; break;
    case GLUT_KEY_UP: c_bottom += pan_speed; c_top += pan_speed; break;
    case GLUT_KEY_DOWN: c_bottom -= pan_speed; c_top -= pan_speed; break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(50, 150);
    glutInitWindowSize(500, 500);
    glutCreateWindow("N-Gon Polygon");

    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(MyKey);
    glutSpecialFunc(SpecialKey);

    glutMainLoop();
    return 0;
}