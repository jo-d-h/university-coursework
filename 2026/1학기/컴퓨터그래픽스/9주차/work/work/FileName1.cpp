#include <windows.h>
#include <cmath>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <ctime>

#define     PI          3.1415926
#define     epsilon     1.0

// 구조체 정의
struct Point { float x, y; };
struct Vector { float x, y; };
struct Color { float r, g, b; };

struct Circle {
    Point   center;
    float   radius;
    float   mass;
    Color   color;
    Vector  velocity;
};

struct Segment {
    Point A;
    Point B;
};

// 전역 변수
int     prev_time = 0;
float   radius = 5.0;
int     width = 600;
int     height = 600;
int     num = 36;       // 원을 그리기 위한 정점 수
int     c_num = 100;    // 공의 개수 100개
Circle* circle;
Point* vertex;
Segment myLine;         // 충돌용 선분

// 벡터 연산 보조 함수
float Dot(Vector v1, Vector v2) { return v1.x * v2.x + v1.y * v2.y; }
float LengthSq(Vector v) { return v.x * v.x + v.y * v.y; }

void init(void) {
    glClearColor(1.0, 0.3, 0.3, 0.0); // 붉은색 배경 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    // 원 그리기용 정점 계산
    vertex = new Point[num];
    for (int i = 0; i < num; i++) {
        float angle = 2.0f * PI * i / num;
        vertex[i].x = cos(angle);
        vertex[i].y = sin(angle);
    }

    // [입력] 선분 위치 (화면을 가로지르는 임의의 선)
    myLine.A = { 100.0f, 150.0f };
    myLine.B = { 500.0f, 450.0f };

    // [원래 코드 방식] 공의 원형 배치 초기화
    circle = new Circle[c_num];
    float Window_radius = width / 3.0f;
    Point Window_Center = { width / 2.0f, height / 2.0f };
    float delta = 2.0f * PI / c_num;

    for (int i = 0; i < c_num; i++) {
        circle[i].radius = radius * (1.5f + (float)std::sin((double)i));
        // 원형 배치
        circle[i].center.x = Window_Center.x + Window_radius * cos((double)(delta * i));
        circle[i].center.y = Window_Center.y + Window_radius * sin((double)(delta * i));
        // 중앙으로 향하는 속도 벡터
        circle[i].velocity.x = -0.5f * (circle[i].center.x - Window_Center.x) / Window_radius;
        circle[i].velocity.y = -0.5f * (circle[i].center.y - Window_Center.y) / Window_radius;

        circle[i].mass = 1.5f + sin((double)(i));
        circle[i].color.r = (float)(1.0 - (i % 3) / 2.0);
        circle[i].color.g = (float)(1.0 - (float)(c_num - i) / c_num);
        circle[i].color.b = (float)(1.0 * (i % 10) / 9.0);

        // 속도 조절 (0.1 곱하기)
        circle[i].velocity.x *= 0.3f;
        circle[i].velocity.y *= 0.3f;
    }
}

// 공-선분 충돌 로직
void Check_Collision_Ball_Line(int idx) {
    Vector AB = { myLine.B.x - myLine.A.x, myLine.B.y - myLine.A.y };
    Vector AC = { circle[idx].center.x - myLine.A.x, circle[idx].center.y - myLine.A.y };

    float ab_len_sq = LengthSq(AB);
    float t = Dot(AC, AB) / ab_len_sq;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    Point P = { myLine.A.x + t * AB.x, myLine.A.y + t * AB.y };
    Vector PC = { circle[idx].center.x - P.x, circle[idx].center.y - P.y };
    float dist = sqrt(LengthSq(PC));

    if (dist < circle[idx].radius) {
        Vector normal = { PC.x / dist, PC.y / dist };
        float v_dot_n = Dot(circle[idx].velocity, normal);

        if (v_dot_n < 0) { // 선분을 향해 이동할 때만 반사
            circle[idx].velocity.x -= (1 + epsilon) * v_dot_n * normal.x;
            circle[idx].velocity.y -= (1 + epsilon) * v_dot_n * normal.y;
        }
    }
}

// 공-공 충돌 (기존 로직)
void Check_Collision_Ball_Ball(int i, int j) {
    Vector normal = { circle[j].center.x - circle[i].center.x, circle[j].center.y - circle[i].center.y };
    float distance = sqrt(normal.x * normal.x + normal.y * normal.y);

    if (distance < circle[j].radius + circle[i].radius) {
        normal.x /= distance;
        normal.y /= distance;

        Vector R_AB = { circle[i].velocity.x - circle[j].velocity.x, circle[i].velocity.y - circle[j].velocity.y };
        float inner_value = Dot(R_AB, normal);

        if (inner_value > 0.0) {
            float impulse = (1 + epsilon) * inner_value / (circle[i].mass + circle[j].mass);
            circle[i].velocity.x -= impulse * circle[j].mass * normal.x;
            circle[i].velocity.y -= impulse * circle[j].mass * normal.y;
            circle[j].velocity.x += impulse * circle[i].mass * normal.x;
            circle[j].velocity.y += impulse * circle[i].mass * normal.y;
        }
    }
}

// 벽 충돌 (기존 로직)
void Check_Collision_Ball_Wall(int idx) {
    if ((circle[idx].center.x + circle[idx].radius > width && circle[idx].velocity.x > 0.0) ||
        (circle[idx].center.x - circle[idx].radius < 0 && circle[idx].velocity.x < 0.0))
        circle[idx].velocity.x *= -1.0;

    if ((circle[idx].center.y + circle[idx].radius > height && circle[idx].velocity.y > 0.0) ||
        (circle[idx].center.y - circle[idx].radius < 0 && circle[idx].velocity.y < 0.0))
        circle[idx].velocity.y *= -1.0;
}

void Update_Position(float delta_time) {
    for (int i = 0; i < c_num; i++) {
        circle[i].center.x += circle[i].velocity.x * delta_time;
        circle[i].center.y += circle[i].velocity.y * delta_time;
    }
}

void RenderScene(void) {
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (float)(current_time - prev_time);
    prev_time = current_time;

    glClear(GL_COLOR_BUFFER_BIT);

    // 1. 충돌 검사
    for (int i = 0; i < c_num; i++) {
        Check_Collision_Ball_Wall(i);
        Check_Collision_Ball_Line(i);
        for (int j = i + 1; j < c_num; j++) {
            Check_Collision_Ball_Ball(i, j);
        }
    }

    // 2. 위치 업데이트
    Update_Position(delta_time);

    // 3. 선분 그리기
    glColor3f(1.0, 1.0, 1.0); // 흰색 선분
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2f(myLine.A.x, myLine.A.y);
    glVertex2f(myLine.B.x, myLine.B.y);
    glEnd();

    // 4. 공 그리기
    for (int i = 0; i < c_num; i++) {
        glColor3f(circle[i].color.r, circle[i].color.g, circle[i].color.b);
        glBegin(GL_POLYGON);
        for (int j = 0; j < num; j++)
            glVertex2f(circle[i].center.x + circle[i].radius * vertex[j].x,
                circle[i].center.y + circle[i].radius * vertex[j].y);
        glEnd();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void SpecialKey(int key, int x, int y) {
    for (int i = 0; i < c_num; i++) {
        if (key == GLUT_KEY_LEFT) { circle[i].velocity.x *= 0.8f; circle[i].velocity.y *= 0.8f; }
        if (key == GLUT_KEY_RIGHT) { circle[i].velocity.x *= 1.2f; circle[i].velocity.y *= 1.2f; }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Line and 100 Balls Collision");
    init();
    prev_time = glutGet(GLUT_ELAPSED_TIME);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKey);
    glutIdleFunc(RenderScene);
    glutMainLoop();
    return 0;
}