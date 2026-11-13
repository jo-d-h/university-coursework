#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

#define	width 			400
#define	height			600
#define	PI				3.1415
#define	polygon_num		50 

int		left_w = 0;
int		bottom_w = 0;
int		collision_count = 1; // 충돌 횟수

typedef struct _Vector { double x; double y; } Vector; //속도, 방향 벡터
typedef struct _Point { double x; double y; } Point;   //좌표 포인트
typedef struct _Ball { Point center; double radius; } Ball;

Ball fixed_ball, moving_ball;
Vector velocity;

void init(void) {
	fixed_ball.radius = 20.0;
	fixed_ball.center.x = width / 2;
	fixed_ball.center.y = height / 2;

	moving_ball.radius = 10.0;
	moving_ball.center.x = width / 2;
	moving_ball.center.y = height / 4;

	velocity = { 0.6, 0.9 }; //초기 이동 속도
}

void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left_w, left_w + width, bottom_w, bottom_w + height);
}

void Modeling_Circle(Ball circle) {
	float delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(circle.center.x + circle.radius * cos(delta * i), circle.center.y + circle.radius * sin(delta * i));
	glEnd();
}

//공과 공 사이의 충돌 및 반응
void Collision_Detection_Between_Balls(void) {
	double dx = moving_ball.center.x - fixed_ball.center.x;
	double dy = moving_ball.center.y - fixed_ball.center.y;
	double dist = sqrt(dx * dx + dy * dy); //중심 간 거리

	//두 반지름의 합보다 거리가 짧으면 충돌
	if (dist <= moving_ball.radius + fixed_ball.radius) {
		double nx = dx / dist;
		double ny = dy / dist;

		//겹침 해결
		double overlap = (moving_ball.radius + fixed_ball.radius) - dist;
		moving_ball.center.x += nx * overlap;
		moving_ball.center.y += ny * overlap;

		//반사 속도 계산
		double dot = velocity.x * nx + velocity.y * ny;
		if (dot < 0) { //공이 서로 다가올 때만 반사
			velocity.x -= 2 * dot * nx;
			velocity.y -= 2 * dot * ny;
			collision_count++;
		}
	}
}

//벽과의 충돌 탐지 및 반응
void Collision_Detection_to_Walls(void) {
	//좌우 벽 체크
	if (moving_ball.center.x + moving_ball.radius >= width) {
		moving_ball.center.x = width - moving_ball.radius;
		velocity.x *= -1;
	}
	else if (moving_ball.center.x - moving_ball.radius <= 0) {
		moving_ball.center.x = moving_ball.radius;
		velocity.x *= -1;
	}

	//상하 벽 체크
	if (moving_ball.center.y + moving_ball.radius >= height) {
		moving_ball.center.y = height - moving_ball.radius;
		velocity.y *= -1;
	}
	else if (moving_ball.center.y - moving_ball.radius <= 0) {
		moving_ball.center.y = moving_ball.radius;
		velocity.y *= -1;
	}
}

void RenderScene(void) {
	glClearColor(1.0, 1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//빨간 공 그리기 (충돌 횟수가 홀수일 때만 노출)
	glColor3f(1.0, 0.0, 0.0);
	if (collision_count % 2)
		Modeling_Circle(fixed_ball);

	//파란 공 위치
	moving_ball.center.x += velocity.x;
	moving_ball.center.y += velocity.y;

	Collision_Detection_Between_Balls();
	Collision_Detection_to_Walls();

	//파란 공 그리기
	glColor3f(0.0, 0.0, 1.0);
	Modeling_Circle(moving_ball);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Bouncing Ball & Wall");
	init();
	glutIdleFunc(RenderScene);
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}