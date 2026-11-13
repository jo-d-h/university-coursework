#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)
#include <cmath>
#include <iostream>

//윈도우 기본 크기
#define	window_width 	400
#define	window_height	600

//원주율, 원 그릴 때 분할 개수
#define	PI				3.1415
#define	polygon_num		30

//미사용
int cc = 0;

//확대 및 축소 비율
float	zoom_factor = 1.0f;

//현재 화면에 보이는 좌표계 크기
float	now_width, now_height;

//현재 영역의 왼쪽과 아래쪽 좌표
float	left, bottom;

//미사용
int		mouse_motion = 0;

//원과 별의 반지름
float	radius = 20.0f;

//미사용
int		point_num = 0;

//좌표 저장 구조체
struct Point { 
	float	x;
	float	y;
};

//중심 좌표, 확대/축소 기준 좌표
Point	circle_center, zoom_point; 

//초기 설정
void init(void) {
	//초기에는 윈도우 크기만큼 보이도록 설정
	now_width = window_width;
	now_height = window_height;

	//좌표계의 중심이 원점이 되도록 설정
	left = -1.0/2.0 * now_width; 
	bottom = -1.0/2.0 * now_height;
}

//좌표 축
void Modeling_Axis(void) {
	glLineWidth(2.0);
	glBegin(GL_LINES);
	//x축
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(0, 0);
	glVertex2i(1000, 0);
	//y축
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 1000);
	glEnd();
}

//사각형
void Modeling_Rectangle(void) {

	glColor3f(1.0, 1.0, 0.0);
	glPointSize(3.0);
	glBegin(GL_POLYGON);
		glVertex2f( 50.0,  50.0);
		glVertex2f(100.0,  50.0);
		glVertex2f(100.0, 100.0);
		glVertex2f( 50.0, 100.0);
	glEnd();
}

//삼각형
void Modeling_Triangle(void) {

	glColor3f(0.0, 1.0, 1.0);
	glPointSize(3.0);
	glBegin(GL_POLYGON);
	glVertex2f( -100.0, 50.0);
	glVertex2f(-150.0, -50.0);
	glVertex2f( -50.0, -50.0);
	glEnd();
}

//별
void	Modeling_Star(void) {
	float	delta;
	int		star_num = 5;

	glColor3f(1.0, 0.0, 1.0);
	glPointSize(3.0);
	delta = 2 * PI / star_num;
	glBegin(GL_LINE_LOOP);

	// 별은 각도를 2배로 건너뛰면서 연결
	for (int i = 0; i < star_num; i++)
		glVertex2f(100.0 + 2.0 * radius * std::cos(2 * i * delta), -100.0 + 2.0 * radius * std::sin(2 * i * delta));
	glEnd();
}

//원
void	Modeling_Circle(void) {
	float	delta;

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(3.0);
	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f( radius * std::cos(delta * i),  radius * std::sin(delta * i));
	glEnd();
}

int win_w = window_width;
int win_h = window_height;

//화면 크기 변경
void Reshape(int w, int h) {
	win_w = w;
	win_h = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//렌더링
void RenderScene(void) {
	glClearColor(0.5, 0.5, 0.6, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((double)left, (double)(left + now_width), (double)bottom, (double)(bottom + now_height));
	
	Modeling_Axis();
	Modeling_Rectangle();
	Modeling_Triangle();
	Modeling_Star();
	Modeling_Circle();

	glFlush();
}

//마우스
void mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) { //마우스 버튼이 눌렸을 때
		zoom_point.x = left + now_width / (float)win_w * x;
		zoom_point.y = bottom + now_height / (float)win_h * (win_h - y);

		if (button == GLUT_LEFT_BUTTON) //왼쪽 버튼 클릭 시 확대
			zoom_factor = 0.9f;
		else if (button == GLUT_RIGHT_BUTTON) //오른쪽 버튼 클릭 시 축소
			zoom_factor = 1.1f;

		now_width *= zoom_factor;
		now_height *= zoom_factor;

		left = zoom_point.x - 1 / 2.0 * now_width;
		bottom = zoom_point.y - 1 / 2.0 * now_height;

		glutPostRedisplay();
	}
}

//방향키 이동
void Special_key(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:		left -= 0.1; break;
	case GLUT_KEY_RIGHT:	left += 0.1; break;
	case GLUT_KEY_DOWN:		bottom -= 0.1; break;
	case GLUT_KEY_UP:		bottom += 0.1; break;
	default: break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(win_w, win_h);
	glutCreateWindow("Zoom_in/out");
	init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(Special_key);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}