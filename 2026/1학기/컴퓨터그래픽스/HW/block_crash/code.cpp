#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

#include "core/Game.h"
#include "core/Settings.h"

namespace {
Game game;

// 화면 다시 그리기
void display()
{
    game.render();
}

// 일정 시간마다 게임 상태 갱신
void update(int)
{
    game.update(Settings::FixedDeltaTime);
        glutPostRedisplay();
    glutTimerFunc(Settings::FrameMilliseconds, update, 0);
}

// 일반 키 입력 처리
void keyboard(unsigned char key, int, int)
{
    game.onKeyDown(key);
}

// 일반 키 입력 해제 처리
void keyboardUp(unsigned char key, int, int)
{
    game.onKeyUp(key);
}

// 특수 키 입력 처리
void specialKey(int key, int, int)
{
    game.onSpecialKeyDown(key);
}

// 특수 키 입력 해제 처리
void specialKeyUp(int key, int, int)
{
    game.onSpecialKeyUp(key);
}

// 2D 좌표계 설정
void reshape(int width, int height)
{
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, Settings::ScreenWidth, Settings::ScreenHeight, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}
}

// GLUT 초기화, 게임 루프 시작
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(Settings::ScreenWidth, Settings::ScreenHeight);
    glutCreateWindow("Gravity Reverse Brick Breaker");

    glClearColor(0.055f, 0.071f, 0.094f, 1.0f);
    game.initialize();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyUp);
    glutTimerFunc(Settings::FrameMilliseconds, update, 0);
    glutMainLoop();

    return 0;
}
