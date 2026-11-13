#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define		Window_Width	800
#define		Window_Height	800

int			point[200][2];
int			num = 0;

double	factorial(int n) {
	double	value;

	if (n <= 1 )
		value = 1;
	else 
		value = n * factorial(n - 1);
	
	return value;
}

double	binomial(int n, int m) {
	double	tt;

	tt = factorial(n) / (factorial(n - m) * factorial(m));
	return tt;
}

double	bernstein(int degree, int index, double t) {
	double	b_value; 

	b_value = binomial(degree, index)
		* pow(1.0 - t, degree - index)
		* pow(t, index);

	return b_value;
}

void Draw_Bezier_Curve(void) {
	double	x, y;
	double	t;
	int		degree = num - 1;

	glColor3f(1.0, 1.0, 0.0);	// yellow
	glLineWidth(2.0);

	glBegin(GL_LINE_STRIP);

	for (t = 0.0; t <= 1.0; t += 0.001) {
		x = 0.0;
		y = 0.0;

		for (int k = 0; k < num; k++) {
			double b = bernstein(degree, k, t);

			x += b * point[k][0];
			y += b * point[k][1];
		}

		glVertex2f((float)x, (float)y);
	}

	glEnd();
}

void Draw_Control_Points(void) {

	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int k = 0; k < num; k++) {
		glVertex2f(point[k][0], point[k][1]);
	}
	glEnd();

}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glColor3f(1.0, 0.0, 0.0);

	if (num >= 1)
		Draw_Control_Points();
	if (num >= 2)
		Draw_Bezier_Curve();

	glFlush();
	glutSwapBuffers();
}
void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION); // Set projection parameters.
	glLoadIdentity();
	gluOrtho2D(0, Window_Width, 0, Window_Height);
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		point[num][0] = x;
		point[num][1] = Window_Height - y;
		num++;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		num = 0;
	}
	glutPostRedisplay();
	RenderScene();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Window_Width, Window_Height);
	glutCreateWindow("Bezier Curve");
	init();
	glutDisplayFunc(RenderScene);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}