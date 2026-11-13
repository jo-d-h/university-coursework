#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <stdlib.h>

#define	width 			400
#define	height			600
#define	PI				3.1415
#define	polygon_num		30

int		left = -50;
int		bottom = -100;

float	move_x = 0.0;
float	move_y = 0.0;

float	radius = 10.0;
float	point[1000][2];
int		point_num = 0;

// 정사각형 색상 변수
float	square_r = 1.0;
float	square_g = 0.0;
float	square_b = 0.0;

typedef struct _Point {
	float	x;
	float	y;
} Point;

Point	circle_center;

// 축 그리기
void Modeling_Axis(void) {
	glLineWidth(2.0);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0); // x축 (빨강)
	glVertex2f(0 + move_x, 0 + move_y);
	glVertex2f(width + move_x, 0 + move_y);

	glColor3f(0.0, 0.0, 1.0); // y축 (파랑)
	glVertex2f(0 + move_x, 0 + move_y);
	glVertex2f(0 + move_x, height + move_y);

	glEnd();
}

// 원 그리기
void	Modeling_Circle(Point CC) {
	float	delta = 2 * PI / polygon_num;

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + move_x + radius * cos(delta * i),
			CC.y + move_y + radius * sin(delta * i));
	glEnd();
}

// 점들 출력
void	Modeling_Points_from_Screen(void) {
	glPointSize(5.0);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_POINTS);
	for (int k = 0; k < point_num; k++)
		glVertex2f(point[k][0] + move_x, point[k][1] + move_y);
	glEnd();
}

// (10,10)에 정사각형
void Modeling_Square(void) {
	glColor3f(square_r, square_g, square_b); // 현재 색상 사용

	glBegin(GL_POLYGON);
	glVertex2f(10 + move_x, 10 + move_y);
	glVertex2f(50 + move_x, 10 + move_y);
	glVertex2f(50 + move_x, 50 + move_y);
	glVertex2f(10 + move_x, 50 + move_y);
	glEnd();
}

void RenderScene(void) {

	// 1. 흰 배경
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height);

	Modeling_Axis();

	// 3. 정사각형
	Modeling_Square();

	// 5. 마우스 위치 원
	Modeling_Circle(circle_center);

	// 4. 기존 점 출력
	Modeling_Points_from_Screen();

	glFlush();
}

// 클릭 이벤트 (기존 유지)
void mouse1(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		float mx = left + (float)x - move_x;
		float my = bottom + (float)(height - y) - move_y;

		// 정사각형 클릭 시 색상 변경
		if (mx >= 10 && mx <= 50 && my >= 10 && my <= 50) {
			square_r = (float)rand() / RAND_MAX;
			square_g = (float)rand() / RAND_MAX;
			square_b = (float)rand() / RAND_MAX;
		}

		point[point_num][0] = mx;
		point[point_num][1] = my;
		point_num++;

		circle_center.x = mx;
		circle_center.y = my;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		point_num = 0;
	}
	glutPostRedisplay();
}

// 5. 마우스 이동 → 원 따라다님
void mouse_move(int x, int y) {
	float mx = left + (float)x - move_x;
	float my = bottom + (float)(height - y) - move_y;

	circle_center.x = mx;
	circle_center.y = my;

	glutPostRedisplay();
}

// 방향키로 이동
void MySpecial(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:		move_x -= 5.0; break;
	case GLUT_KEY_RIGHT:	move_x += 5.0; break;
	case GLUT_KEY_DOWN:		move_y -= 5.0; break;
	case GLUT_KEY_UP:		move_y += 5.0; break;
	default:	break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(-50, -100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Draw Points by Mouse Input");

	glutDisplayFunc(RenderScene);
	glutMouseFunc(mouse1);
	glutPassiveMotionFunc(mouse_move);
	glutSpecialFunc(MySpecial);

	glutMainLoop();
}