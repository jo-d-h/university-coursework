#include <gl/glut.h>
#include <gl/gl.h>
#include <stdio.h>
#include <math.h>

#define	PI	3.1415926

double	c_distance, c_theta, c_phi;
int		viewing_type = 0;

GLfloat	vertices[][3] = {
		{ -1.0, -1.0,  1.0 },		// 0 
		{ -1.0,  1.0,  1.0 },		// 1
		{ 1.0,  1.0,  1.0 },		// 2
		{ 1.0, -1.0,  1.0 },		// 3
		{ -1.0, -1.0, -1.0 },		// 4
		{ -1.0,  1.0, -1.0 },		// 5
		{ 1.0,  1.0, -1.0 },		// 6
		{ 1.0, -1.0, -1.0 } };		// 7

GLfloat	colors[][3] = {
		{ 1.0, 0.0, 0.0 },			// red
		{ 0.0, 1.0, 0.0 },			// green 
		{ 1.0, 1.0, 0.0 },			// yellow
		{ 1.0, 1.0, 1.0 },			// white
		{ 0.0, 0.0, 1.0 },			// blue
		{ 1.0, 0.0, 1.0 } };		// magenta


void camera(void) {

	double x, y, z;
	double ux, uy, uz;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (viewing_type % 2)
		glOrtho(-2.5, 2.5, -2.5, 2.5, 1.0, 10.0);
	else
		gluPerspective(60.0, 1.0, 1.0, 20.0);

	x = c_distance * cos(c_theta) * cos(c_phi);
	y = c_distance * sin(c_theta) * cos(c_phi);
	z = c_distance * sin(c_phi);

	ux = -cos(c_theta) * sin(c_phi);
	uy = -sin(c_theta) * sin(c_phi);
	uz = cos(c_phi);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z,
		0.0, 0.0, 0.0,
		ux, uy, uz);
}


void polygon(int a, int b, int c, int d) {
	glColor3fv(colors[a]);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glEnd();
}
void cube(void) {
	polygon(2, 3, 7, 6);
	polygon(4, 5, 6, 7);
	polygon(3, 0, 4, 7);
	polygon(1, 2, 6, 5);
	polygon(5, 4, 0, 1);
	polygon(0, 3, 2, 1);
}

void init(void)
{
	c_distance = 5.0;
	c_theta = 0.6;
	c_phi = 0.5;

	// Z-buffer 알고리즘 활성화
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 20.0);
}

void axis(void) {

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // y축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0.0, 0.0, 1.0); // z축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}

void frame_reset(void) {
	glClearColor(0.6, 0.6, 0.6, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void display(void)
{
	frame_reset();

	camera();
	axis();
	cube();

	glFlush();
	glutSwapBuffers();
}

void Special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:		c_theta -= 0.03;	break;
	case GLUT_KEY_RIGHT:	c_theta += 0.03;	break;
	case GLUT_KEY_DOWN:		c_phi -= 0.03;	break;
	case GLUT_KEY_UP:		c_phi += 0.03;	break;
	default:	break;
	}
	glutPostRedisplay();
}


void MyKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'v': viewing_type++;		break;
	case 'a': c_distance += 0.1;	break;
	case 's': c_distance -= 0.1;	break;
	default:						break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3D_cube_with_Rotatable_Camera");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(MyKey);
	glutSpecialFunc(Special);
	glutMainLoop();
	return 0;
}
