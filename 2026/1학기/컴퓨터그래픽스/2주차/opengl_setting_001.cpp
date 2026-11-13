#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

void Draw_Axis(void) {
	glLineWidth(3.0);

	glBegin(GL_LINES);
		//가로축(Red)
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glVertex2f(1.0, 0.0);

		//세로축(Green)
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 1.0);
	glEnd();
}

void Draw_Square(void)
{
	glLineWidth(3.0);
	glColor3f(1.0, 1.0, 0.0); //Yellow

	glBegin(GL_POLYGON);
		glVertex2f(0.2, -0.2);
		glVertex2f(0.6, -0.2);
		glVertex2f(0.6, 0.6);
		glVertex2f(0.2, 0.6);
	glEnd();
}

//n각형 그리기 (delta 방식)
void Draw_N_Gon(int n) {
	glLineWidth(3.0);
	glColor3f(1.0, 0.0, 0.0); // Red

	float PI = 3.141592f;
	float delta = 2.0f * PI / n;
	float radius = 0.5f;

	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		float x = radius * cos(delta * i);
		float y = radius * sin(delta * i);
		glVertex2f(x, y);
	}
	glEnd();
}

//선(테두리) : GL_LINE_LOOP / 채우기 : GL_POLYGON  / 삼각형 : GL_TRIANGLES
//선(독립된) : GL_LINES

void RenderScene(void) {
	//배경색 설정 및 초기화
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//축 그리기
	Draw_Axis();

	//사각형 그리기
	Draw_Square();

	//n각형 그리기
	//Draw_N_Gon(17);
	Draw_N_Gon(39);

	//실행
	glFlush();
}


int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Welcome to Computer Graphics");
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}