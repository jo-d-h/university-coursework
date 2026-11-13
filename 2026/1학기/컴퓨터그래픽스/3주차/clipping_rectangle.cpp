#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

#define PI		3.1415926
#define n_gon	30

float	radius = 100.0;
double	delta_theta;

void draw_axis(void) {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(500.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 500.0);
	glEnd();
}

void draw_rectangle(void) {
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(50.0, 50.0);
	glVertex2f(100.0, 50.0);
	glVertex2f(100.0, 100.0);
	glVertex2f(50.0, 100.0);
	glEnd();
}



void draw_Ngon(void) {
	int i;
	glColor3f(1.0, 1.0, 0.0);
	delta_theta = 2 * PI / n_gon;

	glBegin(GL_POLYGON);

	for (i = 0; i < n_gon; i++)
		glVertex2f(radius * cos(delta_theta * i), radius * sin(delta_theta * i));

	glEnd();
}



void RenderScene(void) {
	glClearColor(1.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Clipping Window : x는[100, 400], y는 [-200, 300]
	gluOrtho2D(-100, 400, -200, 300);

	draw_rectangle();
	draw_Ngon();
	draw_axis();

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Clipping Window(Rectangle)");
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}
// 테두리로만 나타난 사각형을 원 위에 놓이게 하기 위해서는 ?