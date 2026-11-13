#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define		PI	3.1415926

int			Width = 800;
int			Height = 800;

float		sun_radius;
float		earth_radius;
float		moon_radius;
float		mars_radius;

float		earthRotation;
float		moonRotation;
float		marsRotation;

float		earth_rotation_speed;
float		moon_rotation_speed;
float		mars_rotation_speed;

float		earth_orbit_radius;
float		moon_orbit_radius;
float		mars_orbit_radius;

void init(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0 * Width / 2.0, Width / 2.0, -1.0 * Height / 2.0, Height / 2.0);
	glMatrixMode(GL_MODELVIEW);

	sun_radius = 50.0;
	earth_radius = 20.0;
	moon_radius = 10.0;
	mars_radius = 15.0;

	// 공전 궤도 반지름
	earth_orbit_radius = 200.0;
	moon_orbit_radius = 50.0;
	mars_orbit_radius = 300.0;

	// 회전 속도 설정
	earth_rotation_speed = 0.03;
	moon_rotation_speed = 0.05;
	mars_rotation_speed = 0.02;

	earthRotation = 0.0;
	moonRotation = 0.0;
	marsRotation = 0.0;
}


void Draw_Circle(float c_radius) {
	float	delta;
	int		num = 36;

	delta = (float)(2 * PI / num);
	glBegin(GL_POLYGON);
	for (int i = 0; i < num; i++)
		glVertex2d(c_radius * cos(delta * i), c_radius * sin(delta * i));
	glEnd();
}



void RenderScene(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0); // Set display-window color to black.
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	earthRotation += earth_rotation_speed;
	moonRotation += moon_rotation_speed;
	marsRotation += mars_rotation_speed;

	if (earthRotation > 360.0) earthRotation -= 360.0;
	if (moonRotation > 360.0) moonRotation -= 360.0;
	if (marsRotation > 360.0) marsRotation -= 360.0;

	// 태양 : 빨간색의 구 (중앙 고정)
	glColor3f(1.0, 0.0, 0.0);
	Draw_Circle(sun_radius);


	/***********코드 작성하기 ***************/

	// ===== 지구 =====
	glPushMatrix();

	glRotatef(earthRotation, 0.0, 0.0, 1.0);
	glTranslatef(earth_orbit_radius, 0.0, 0.0);

	// 지구
	glColor3f(0.0, 1.0, 0.0);
	Draw_Circle(earth_radius);


	// 달
	glPushMatrix();

	glRotatef(moonRotation, 0.0, 0.0, 1.0);
	glTranslatef(moon_orbit_radius, 0.0, 0.0);

	// 달
	glColor3f(1.0, 1.0, 0.0);
	Draw_Circle(moon_radius);

	glPopMatrix(); // 지구 위치로 복원
	glPopMatrix(); // 태양 중심으로 복원


	// 화성
	glPushMatrix();

	glRotatef(marsRotation, 0.0, 0.0, 1.0);
	glTranslatef(mars_orbit_radius, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	Draw_Circle(mars_radius);

	glPopMatrix();

	/***************************************/

	glFlush();
	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutMainLoop();
	return 0;
}


