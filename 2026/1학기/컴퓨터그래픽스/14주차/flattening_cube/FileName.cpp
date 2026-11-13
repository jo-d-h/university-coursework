// 정육면체의 4개 옆면을 바깥쪽으로 펼치는 예제

#include <gl/glut.h>
#include <gl/gl.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926

GLfloat vertices[][3] = {
	{ -1.0, -1.0,  1.0 },		// 0
	{ -1.0,  1.0,  1.0 },		// 1
	{  1.0,  1.0,  1.0 },		// 2
	{  1.0, -1.0,  1.0 },		// 3
	{ -1.0, -1.0, -1.0 },		// 4
	{ -1.0,  1.0, -1.0 },		// 5
	{  1.0,  1.0, -1.0 },		// 6
	{  1.0, -1.0, -1.0 }		// 7
};

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },			// 빨강
	{ 0.0, 1.0, 0.0 },			// 초록
	{ 1.0, 1.0, 0.0 },			// 노랑
	{ 1.0, 1.0, 1.0 },			// 흰색
	{ 0.0, 0.0, 1.0 },			// 파랑
	{ 1.0, 0.0, 1.0 }			// 자홍색
};

int view_type = 0;
int animate_sign;

float up = 1.0;
float animate_time, delta_time;

double c_distance, c_theta, c_phi;


// 사각형 면 그리기
void polygon(int a, int b, int c, int d)
{
	glColor3fv(colors[a]);

	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glEnd();
}


// 정육면체 그리기
void cube(void)
{
	// 바닥면은 고정
	polygon(4, 5, 6, 7);

	// 뒤쪽 옆면
	// 바닥면의 모서리 5-6을 기준으로 바깥쪽으로 회전
	glPushMatrix();
	glTranslatef(0.0, 1.0, -1.0);
	glRotatef(-animate_time, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -1.0, 1.0);
	polygon(1, 2, 6, 5);
	glPopMatrix();


	// 왼쪽 옆면
	// 바닥면의 모서리 4-5를 기준으로 바깥쪽으로 회전
	glPushMatrix();
	glTranslatef(-1.0, 0.0, -1.0);
	glRotatef(-animate_time, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 1.0);
	polygon(5, 4, 0, 1);
	glPopMatrix();


	// 오른쪽 옆면
	// 바닥면의 모서리 6-7을 기준으로 바깥쪽으로 회전
	glPushMatrix();
	glTranslatef(1.0, 0.0, -1.0);
	glRotatef(+animate_time, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, 1.0);
	polygon(2, 3, 7, 6);
	glPopMatrix();


	// 앞쪽 옆면
	// 바닥면의 모서리 4-7을 기준으로 바깥쪽으로 회전
	glPushMatrix();
	glTranslatef(0.0, -1.0, -1.0);
	glRotatef(animate_time, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 1.0, 1.0);
	polygon(3, 0, 4, 7);
	glPopMatrix();


	// 윗면
	polygon(0, 3, 2, 1);
}


void init(void)
{
	c_distance = 7.0;
	c_theta = 0.3;
	c_phi = 0.4;

	animate_sign = 0;
	animate_time = 0.0;
	delta_time = 0.1;

	glEnable(GL_DEPTH_TEST);

	// 뒷면 제거 기능
	// 면이 사라져 보일 수 있으므로 기본적으로 꺼둠
	//glEnable(GL_CULL_FACE);
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, 1.0, 1.0, 20.0);
}


// 좌표축 그리기
void axis(void)
{
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);	// x축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);	// y축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);	// z축
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);

	glEnd();
}


// 화면 초기화
void frame_reset(void)
{
	glClearColor(0.6, 0.6, 0.6, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// 카메라 설정
void camera(void)
{
	float x, y, z;

	x = (float)(c_distance * cos(c_theta) * cos(c_phi));
	y = (float)(c_distance * sin(c_theta) * cos(c_phi));
	z = (float)(c_distance * sin(c_phi));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		x, y, z,
		0.0, 0.0, 0.0,
		0.0, 0.0, up
	);
}


void display(void)
{
	// t 키를 누르면 애니메이션 시작/정지
	if (animate_sign % 2 == 1)
	{
		animate_time += delta_time;

		// 옆면이 완전히 펼쳐지는 각도는 90도
		if (animate_time > 90.0)
			animate_time = 90.0;
	}

	frame_reset();
	camera();

	axis();
	cube();

	glFlush();
	glutSwapBuffers();
}


// 일반 키보드 입력 처리
void MyKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		glFrontFace(GL_CCW);
		break;

	case 's':
		glFrontFace(GL_CW);
		break;

	case 't':
		animate_sign++;
		break;

	case 'n':
		animate_time = 0.0;
		animate_sign = 0;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


// 특수 키 입력 처리
void Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		c_theta -= 0.01;
		break;

	case GLUT_KEY_RIGHT:
		c_theta += 0.01;
		break;

	case GLUT_KEY_DOWN:
		c_phi -= 0.01;
		break;

	case GLUT_KEY_UP:
		c_phi += 0.01;
		break;

	default:
		break;
	}

	if (c_phi > 3.0 * PI / 2.0)
		c_phi -= 2.0 * PI;
	else if (c_phi < -3.0 * PI / 2.0)
		c_phi += 2.0 * PI;

	if (c_phi > PI / 2.0)
		up = -1.0;
	else if (c_phi < -1.0 * PI / 2.0)
		up = -1.0;
	else
		up = 1.0;

	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Flattening_Cube");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(MyKey);
	glutIdleFunc(display);
	glutSpecialFunc(Special);

	glutMainLoop();

	return 0;
}