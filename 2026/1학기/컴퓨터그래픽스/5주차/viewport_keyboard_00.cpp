#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h>

#define PI 3.1415926
#define Window_Width 1000
#define Window_Height 1000
#define MAX 1000

int num = 30;
float radius = 30;

// 마우스로 찍은 원
float cx[MAX];
float cy[MAX];
int count = 0;

// 화면 이동
float offsetX = 0;
float offsetY = 0;

//viewport 크기
int win_w = Window_Width;
int win_h = Window_Height;

void Draw_Circle(float cx0, float cy0) {
    float delta = 2 * PI / num;

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < num; i++) {
        float theta = delta * i;
        float x = cx0 + radius * cos(theta);
        float y = cy0 + radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void Modeling_Circle(void) {
    Draw_Circle(0, 0);
}

void Modeling_Rectangle(void) {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(100, 100);
    glVertex2i(150, 100);
    glVertex2i(150, 150);
    glVertex2i(100, 150);
    glEnd();
}

void Modeling_Axis(void) {
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-250.0, 0.0);
    glVertex2f(250.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0, -250.0);
    glVertex2f(0, 250.0);

    glEnd();
}

void Modeling_Ground(void) {
    glColor3f(0.0, 0.5, 0.5);
    glBegin(GL_POLYGON);
    glVertex2i(250, 250);
    glVertex2i(-250, 250);
    glVertex2i(-250, -250);
    glVertex2i(250, -250);
    glEnd();
}

void RenderScene(void) {
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //  같이 이동하는 것들
    glPushMatrix();
    glTranslatef(offsetX, offsetY, 0.0);

    Modeling_Ground();
    Modeling_Rectangle();
    Modeling_Axis();
    Modeling_Circle();

    glPopMatrix();

    //  고정된 원들
    for (int i = 0; i < count; i++) {
        Draw_Circle(cx[i], cy[i]);
    }

    glFlush();
}

void Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {

        float fx = x - win_w / 2;
        float fy = win_h / 2 - y;

        // 이동 보정
        fx -= offsetX;
        fy -= offsetY;

        if (button == GLUT_LEFT_BUTTON) {
            if (count < MAX) {
                cx[count] = fx;
                cy[count] = fy;
                count++;
            }
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            count = 0;
        }

        glutPostRedisplay();
    }
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1': offsetX = -150; offsetY = -150; break;
    case '2': offsetX = 150;  offsetY = -150; break;
    case '3': offsetX = 150;  offsetY = 150;  break;
    case '4': offsetX = -150; offsetY = 150;  break;
    case '5': offsetX = 0;    offsetY = 0;    break;
    }

    glutPostRedisplay();
}

void MyReshape(int w, int h) {
    win_w = w;
    win_h = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(Window_Width, Window_Height);
    glutCreateWindow("Final Complete");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
    return 0;
}