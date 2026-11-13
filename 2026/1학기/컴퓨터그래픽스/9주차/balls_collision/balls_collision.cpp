#include <windows.h>
#include <cmath>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h> // (or others, depending on the system in use)
#include <ctime>


#define		PI			3.1415926
#define		epsilon		1.0


int prev_time = 0;



// Size of Clipping Window
float	radius = 5.0;
int		width = 500;
int		height = 500;
float	point_size = 3.0;
int		n = 0;

// Parameters of a Sphere
int		num = 36;
//int		c_num = 2;
int		c_num = 100;
float	delta;
GLenum  draw_type;
int		collide = 0;

struct Point {
	float	x;
	float	y;
};

struct Vector {
	float	x;
	float	y;
};

struct Color {
	float	r;
	float	g;
	float	b;
};

struct Circle {
	Point	center;
	float	radius;
	float	mass;
	Color	color;
	Vector	velocity;
};

Circle* circle;
Point* vertex;

float		delta_x, delta_y;
float		fix_radius;

Point	Window_Center;
float	Window_radius;




void init(void) {
	glClearColor(1.0, 0.3, 0.3, 0.0); // Set display-window color to white.
	glMatrixMode(GL_PROJECTION); // Set projection parameters.
	gluOrtho2D(0, width, 0, height);
	draw_type = GL_LINES;

	vertex = new Point[num];
	delta = 2.0 * PI / num;
	for (int i = 0; i < num; i++) {
		vertex[i].x = cos(delta * i);
		vertex[i].y = sin(delta * i);
	}

	Window_radius = width / 3.0;
	Window_Center.x = width / 2.0;
	Window_Center.y = height / 2.0;

	delta = 2.0 * PI / c_num;
	circle = new Circle[c_num];

	for (int i = 0; i < c_num; i++) {
		circle[i].radius = radius * (1.5f + (float)std::sin((double)i));
		circle[i].center.x = Window_Center.x + Window_radius * cos((double)(delta * i));
		circle[i].center.y = Window_Center.y + Window_radius * sin((double)(delta * i));
		circle[i].velocity.x = -0.5 * (circle[i].center.x - Window_Center.x) / Window_radius;
		circle[i].velocity.y = -0.5 * (circle[i].center.y - Window_Center.y) / Window_radius;
		circle[i].mass = 1.5 + sin((double)(i));
		circle[i].color.r = (float)(1.0 - (i % 3) / 2);
		circle[i].color.g = (float)(1.0 - (c_num - i) / c_num);
		circle[i].color.b = (float)(1.0 * (i % 10) / 9);

		circle[i].velocity.x *= 0.1;
		circle[i].velocity.y *= 0.1;
	}

}



void	Draw_Circle(int	index) {
	float	x, y, radius;

	x = circle[index].center.x;
	y = circle[index].center.y;
	radius = circle[index].radius;
	glColor3f(circle[index].color.r, circle[index].color.g, circle[index].color.b);
	glBegin(GL_POLYGON);
	for (int i = 0; i < num; i++)
		glVertex3f(x + radius * vertex[i].x, y + radius * vertex[i].y, 0.0);

	glEnd();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void	Check_Collision_Ball_Wall(int index) {
	if (circle[index].center.x + circle[index].radius > width && circle[index].velocity.x > 0.0)
		circle[index].velocity.x *= (-1.0);

	if (circle[index].center.x - circle[index].radius < 0 && circle[index].velocity.x < 0.0)
		circle[index].velocity.x *= (-1.0);


	if (circle[index].center.y + circle[index].radius > height && circle[index].velocity.y > 0.0)
		circle[index].velocity.y *= (-1.0);

	if (circle[index].center.y - circle[index].radius < 0 && circle[index].velocity.y < 0.0)
		circle[index].velocity.y *= (-1.0);
}


void	Check_Collision_Ball_Ball(int i, int j) {
	float	distance;
	Vector	VA, VB, R_AB, normal;
	float	inner_value;

	normal.x = circle[j].center.x - circle[i].center.x;
	normal.y = circle[j].center.y - circle[i].center.y;

	distance = sqrt(normal.x * normal.x + normal.y * normal.y);

	if (distance < circle[j].radius + circle[i].radius) {
		distance = sqrt(normal.x * normal.x + normal.y * normal.y);

		normal.x /= distance;
		normal.y /= distance;


		VA.x = circle[i].velocity.x;
		VA.y = circle[i].velocity.y;

		VB.x = circle[j].velocity.x;
		VB.y = circle[j].velocity.y;

		R_AB.x = VA.x - VB.x;
		R_AB.y = VA.y - VB.y;

		inner_value = R_AB.x * normal.x + R_AB.y * normal.y;
		if (inner_value > 0.0) {
			circle[i].velocity.x = VA.x - (1 + epsilon) * circle[j].mass * inner_value * normal.x / (circle[i].mass + circle[j].mass);
			circle[i].velocity.y = VA.y - (1 + epsilon) * circle[j].mass * inner_value * normal.y / (circle[i].mass + circle[j].mass);

			circle[j].velocity.x = VB.x + (1 + epsilon) * circle[i].mass * inner_value * normal.x / (circle[i].mass + circle[j].mass);
			circle[j].velocity.y = VB.y + (1 + epsilon) * circle[i].mass * inner_value * normal.y / (circle[i].mass + circle[j].mass);

			collide = 1;
		}
	}

}


void Update_Position(float delta_time) {
	for (int i = 0; i < c_num; i++) {
		circle[i].center.x += circle[i].velocity.x * delta_time;
		circle[i].center.y += circle[i].velocity.y * delta_time;
	}
}


void RenderScene(void) {
	int		i;

	int current_time = glutGet(GLUT_ELAPSED_TIME);

	//float delta_time = (current_time - prev_time) / 1000.0f; // ÃÊ ´ÜÀ§
	
	float delta_time = (current_time - prev_time)  ; 
	prev_time = current_time;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);


	for (i = 0; i < c_num; i++)
		Check_Collision_Ball_Wall(i);

	Update_Position(delta_time);

	for (i = 0; i < c_num; i++) {
		for (int j = i + 1; j < c_num; j++) {
			if (i != j)
				Check_Collision_Ball_Ball(i, j);
		}
	}

	Update_Position(delta_time);

	for (i = 0; i < c_num; i++)
		Draw_Circle(i);

	glFlush();
	glutSwapBuffers();
}



void SpecialKey(int key, int x, int y) {
	int		i;

	switch (key) {
	case GLUT_KEY_LEFT:
		for (i = 0; i < c_num; i++) {
			circle[i].velocity.x *= 0.1;
			circle[i].velocity.y *= 0.1;
		}
		break;
	case GLUT_KEY_RIGHT:
		for (i = 0; i < c_num; i++) {
			circle[i].velocity.x *= 1.5;
			circle[i].velocity.y *= 1.5;
		}
		break;
	default:				break;
	}
}

void IdleFunction(void) {
	RenderScene();
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Ball_Ball_Collision");
	init();
	prev_time = glutGet(GLUT_ELAPSED_TIME);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(MyReshape);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(IdleFunction);
	glutMainLoop();
	return 0;
}