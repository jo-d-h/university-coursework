#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define		Window_Width	800
#define		Window_Height	800

int			control_point[100][2];
int			num = 0;
int			function_type = 0;


void BezierPoint(double t, double* x, double* y) {
	double temp[100][2];

	for (int i = 0; i < num; i++) {
		temp[i][0] = control_point[i][0];
		temp[i][1] = control_point[i][1];
	}

	for (int r = 1; r < num; r++) {
		for (int i = 0; i < num - r; i++) {
			temp[i][0] = (1.0 - t) * temp[i][0] + t * temp[i + 1][0];
			temp[i][1] = (1.0 - t) * temp[i][1] + t * temp[i + 1][1];
		}
	}

	*x = temp[0][0];
	*y = temp[0][1];
}


void Draw_Bezier_Curve2(void) {
	double x, y;
	int curvepoint_num = 100;

	glColor3f(1.0, 1.0, 0.0);

	glBegin(GL_LINE_STRIP);

	for (int i = 0; i <= curvepoint_num; i++) {
		double t = (double)i / curvepoint_num;
		BezierPoint(t, &x, &y);
		glVertex2d(x, y);
	}

	glEnd();
}


int	Factorial(int k) {

	int	value;

	if (k <= 1)
		value = 1;
	else {
		value = k * Factorial(k - 1);
	}
	return value;
}

double Combination(int n, int k) {
	double	value;
	if (n >= 1 && k <= n)
		value = Factorial(n) / (Factorial(n - k) * Factorial(k));
	else
		value = 1;
	return value;
}

double Bernstein(int n, int k, double time) {
	double value;
	if (0.0 <= time && time <= 1.0)
		value = Combination(n, k) * pow(1.0 - time, n - k) * pow(time, k);
	else
		value = 1.0;
	return value;
}

void Draw_Bezier_Curve1(void) {
	double	px, py;
	double	sumx, sumy;
	double	time;
	double	function_value;
	double  partial_time;
	int		curvepoint_num = 100;
	int		curve_degree;

	glColor3f(1.0, 1.0, 0.0);

	curve_degree = num - 1;
	partial_time = 1.0 / curvepoint_num;

	px = control_point[0][0];
	py = control_point[0][1];

	glBegin(GL_LINE_STRIP);
	for (time = 0.0; time <= 1.0; time += partial_time) {
		sumx = sumy = 0.0;
		for (int k = 0; k <= curve_degree; k++) {
			function_value = Bernstein(curve_degree, k, time);
			sumx += function_value * control_point[k][0];
			sumy += function_value * control_point[k][1];
		}
		glVertex2d(sumx, sumy);
		px = sumx;
		py = sumy;
	}
	sumx = control_point[curve_degree][0];
	sumy = control_point[curve_degree][1];

	glVertex2d(px, py);
	glVertex2d(sumx, sumy);
	glEnd();
}
void Draw_Control_Points(void) {

	glPointSize(3.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int k = 0; k < num; k++) {
		glVertex2i(control_point[k][0], control_point[k][1]);
	}
	glEnd();

}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glColor3f(1.0, 0.0, 0.0);

	if (num >= 1)
		Draw_Control_Points();
	if (num >= 2) {
		if (function_type)
			Draw_Bezier_Curve1();
		else
			Draw_Bezier_Curve2();
	}

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
		control_point[num][0] = x;
		control_point[num][1] = Window_Height - y;
		num++;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		num = 0;
	}
	glutPostRedisplay();
	RenderScene();
}

void mykey(unsigned char key, int x, int y) {
	switch (key) {
	case 'f':	function_type = !function_type;	break;
	default:	break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Window_Width, Window_Height);
	glutCreateWindow("Bezier Curve");
	init();
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(mykey);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}