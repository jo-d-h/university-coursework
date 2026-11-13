#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

#define		PI	3.1415

int		polygon_num = 4;
float	radius = 100;


void  Modeling_Polygon(void) {
	float	x, y;
	double	delta;

	delta = 2 * PI / polygon_num;

	 //색상 조정
	glPointSize(10.0);
	
	//삼각형
    glBegin(GL_LINE_LOOP);
    
	for (int i = 0; i < polygon_num; i++) {
		x = radius * cos(i * delta);
		y = radius * sin(i * delta);
		glVertex2f(x, y);
		}
}

void Modeling_Axis(void) 
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(-250, 0);
	glVertex2i(250, 0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex2i(0, 250);
	glVertex2i(0, -250);
	glEnd();

}

void Camera_Lense_Setting(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0); //3차원을 2차원으로 투영
	//glOrtho(-250.0, 250.0, -250.0, 250.0,-1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void RenderScene(void) { //배경색 지정
	glClearColor(0.0, 0.0, 0.0, 0.0); //검정
	glClear(GL_COLOR_BUFFER_BIT); //화면 검정으로 초기화

    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glut.h>
    #include <math.h>

    #define		PI	3.1415

    int		polygon_num = 4;
    float	radius = 100;

    void  Modeling_Polygon(void) {
        float	x, y;
        double	delta;

        delta = 2 * PI / polygon_num;

        glColor3f(0.0, 1.0, 0.0); // 초록색
        glPointSize(10.0);

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < polygon_num; i++) {
            x = radius * cos(i * delta);
            y = radius * sin(i * delta);
            glVertex2f(x, y);
        }
        glEnd(); // glBegin에 대응하는 glEnd 추가
    }

    void Modeling_Axis(void) 
    {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2i(-250, 0);
        glVertex2i(250, 0);
        glEnd();

        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2i(0, 250);
        glVertex2i(0, -250);
        glEnd();
    }

    void Camera_Lense_Setting(void) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void RenderScene(void) {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        Camera_Lense_Setting();
        Modeling_Axis();
        Modeling_Polygon();

        glFlush();
    }

    int main(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitWindowPosition(50, 150);
        glutInitWindowSize(500, 500);
        glutCreateWindow("Welcome to Computer Graphics");
        glutDisplayFunc(RenderScene);
        glutMainLoop();
        return 0;
    }
	Camera_Lense_Setting(); //카메라	렌즈 설정
	Modeling_Axis(); //좌표축 2개 그리기
	Modeling_Polygon(); //다각형 그리기


	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 150); //화면 위치(y값 커질수록 아래로 내려감)
	glutInitWindowSize(500, 500); //화면 크기
	glutCreateWindow("Welcome to Computer Graphics");
	glutDisplayFunc(RenderScene);
	glutMainLoop(); //매우 중요! 없으면 창이 바로 꺼짐. 
    void Modeling_Polygon(void) 
	{
       float x, y;
       double delta;

       delta = 2 * PI / polygon_num;

       glColor3f(0.0, 1.0, 0.0); // 초록색 추가
       glPointSize(10.0);

       glBegin(GL_LINE_LOOP);
       for (int i = 0; i < polygon_num; i++) {
           x = radius * cos(i * delta);
           y = radius * sin(i * delta);
		   glVertex2f(x, y);
       }
       glEnd(); // glBegin에 대응하는 glEnd 추가
    }
        void  Modeling_Polygon(void) {
            float x, y;
            double delta;

            delta = 2 * PI / polygon_num;

            glColor3f(0.0, 1.0, 0.0); // 초록색 추가
            glPointSize(10.0);

            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < polygon_num; i++) {
                x = radius * cos(i * delta);
                y = radius * sin(i * delta);
                glVertex2f(x, y);
            }
            glEnd(); // glBegin에 대응하는 glEnd 추가
        }
	//이벤트 루프 시작, 이벤트가 발생할 때까지 대기, 
	//이벤트가 발생하면 콜백 함수 호출
	//(타 언어와 달리	C++에서는 이벤트 루프가 없으면 창이 바로 꺼짐)
	return 0;
}