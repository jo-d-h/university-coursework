#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <ctime>

// 설정값
int width = 800, height = 600;
const int BALL_COUNT = 100;
const float E = 1.0f; // 반발 계수 (1.0 = 완전 탄성 충돌) [cite: 204]

struct Ball {
    float x, y, vx, vy, radius, mass;
    float r, g, b;
};

std::vector<Ball> balls;

void init() {
    for (int i = 0; i < BALL_COUNT; ++i) {
        Ball b;
        b.radius = 5.0f + (rand() % 10);
        b.mass = b.radius; // 질량을 반지름에 비례하게 설정
        b.x = b.radius + (rand() % (int)(width - b.radius * 2));
        b.y = b.radius + (rand() % (int)(height - b.radius * 2));
        b.vx = ((rand() % 10) - 5) * 0.5f;
        b.vy = ((rand() % 10) - 5) * 0.5f;
        b.r = (rand() % 255) / 255.0f;
        b.g = (rand() % 255) / 255.0f;
        b.b = (rand() % 255) / 255.0f;
        balls.push_back(b);
    }
}

void update(int value) {
    for (int i = 0; i < balls.size(); ++i) {
        // 1. 위치 업데이트 [cite: 1]
        balls[i].x += balls[i].vx;
        balls[i].y += balls[i].vy;

        // 2. 공 & 벽 충돌 탐지 및 반응 [cite: 3]
        if (balls[i].x - balls[i].radius < 0 || balls[i].x + balls[i].radius > width) {
            balls[i].vx *= -1; // 방향 반전
            balls[i].x = (balls[i].x < width / 2) ? balls[i].radius : width - balls[i].radius;
        }
        if (balls[i].y - balls[i].radius < 0 || balls[i].y + balls[i].radius > height) {
            balls[i].vy *= -1;
            balls[i].y = (balls[i].y < height / 2) ? balls[i].radius : height - balls[i].radius;
        }

        // 3. 공 & 공 충돌 탐지 및 반응 [cite: 1, 205]
        for (int j = i + 1; j < balls.size(); ++j) {
            float dx = balls[j].x - balls[i].x;
            float dy = balls[j].y - balls[i].y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance <= balls[i].radius + balls[j].radius) { // 충돌 조건 [cite: 2]
                // 법선 벡터(Normal) 계산
                float nx = dx / distance;
                float ny = dy / distance;

                // 위치 보정 (겹침 현상 해결)
                float overlap = (balls[i].radius + balls[j].radius) - distance;
                balls[i].x -= nx * overlap * 0.5f;
                balls[i].y -= ny * overlap * 0.5f;
                balls[j].x += nx * overlap * 0.5f;
                balls[j].y += ny * overlap * 0.5f;

                // 운동량 보존 및 반발 계수 적용 [cite: 205]
                float rvx = balls[i].vx - balls[j].vx;
                float rvy = balls[i].vy - balls[j].vy;
                float velAlongNormal = rvx * nx + rvy * ny;

                if (velAlongNormal > 0) continue; // 멀어지는 경우 제외

                float j_impulse = -(1.0f + E) * velAlongNormal;
                j_impulse /= (1.0f / balls[i].mass + 1.0f / balls[j].mass);

                balls[i].vx += (j_impulse / balls[i].mass) * nx;
                balls[i].vy += (j_impulse / balls[i].mass) * ny;
                balls[j].vx -= (j_impulse / balls[j].mass) * nx;
                balls[j].vy -= (j_impulse / balls[j].mass) * ny;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (const auto& b : balls) {
        glColor3f(b.r, b.g, b.b);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 20; i++) {
            float th = i * 2.0f * 3.14159f / 20.0f;
            glVertex2f(b.x + cos(th) * b.radius, b.y + sin(th) * b.radius);
        }
        glEnd();
    }
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Physics Ball Collision");
    init();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}