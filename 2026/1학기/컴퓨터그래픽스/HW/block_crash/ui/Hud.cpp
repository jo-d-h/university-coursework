#include "Hud.h"

#include <cstdio>
#include <string>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

#include "../core/Settings.h"

namespace
{
// 텍스트 중앙 정렬
void drawCenteredText(float y, const std::string& text)
{
    const float characterWidth = 9.0f;
    const float x = (Settings::ScreenWidth - text.length() * characterWidth) * 0.5f;
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

// 텍스트 위치 지정, 출력
void drawText(float x, float y, const std::string& text)
{
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

// 텍스트 박스 배경
void drawCenteredNotice(float y, const std::string& text)
{
    const float characterWidth = 9.0f;
    const float textWidth = text.length() * characterWidth;
    const float left = (Settings::ScreenWidth - textWidth) * 0.5f - 12.0f;
    const float right = left + textWidth + 24.0f;
    const float top = y - 17.0f;
    const float bottom = y + 5.0f;

    glColor3f(0.12f, 0.15f, 0.19f);
    glBegin(GL_QUADS);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();

    glColor3f(0.9f, 0.93f, 0.97f);
    drawCenteredText(y, text);
}
}

// HUD 출력
void Hud::draw(
    int score,
    int highScore,
    bool gameOver,
    bool ballAttached,
    const std::vector<ActiveEffectNotice>& activeEffectNotices)
{
    glColor3f(0.9f, 0.93f, 0.97f);
    const std::string scoreText = "Score " + std::to_string(score);
    const std::string highScoreText = "High Score " + std::to_string(highScore);
    drawText(24.0f, 34.0f, scoreText);
    drawText(24.0f, 54.0f, highScoreText);

    if (gameOver)
    {
        const float panelLeft = Settings::ScreenWidth * 0.5f - 150.0f;
        const float panelRight = Settings::ScreenWidth * 0.5f + 150.0f;
        const float panelTop = Settings::ScreenHeight * 0.5f - 42.0f;
        const float panelBottom = Settings::ScreenHeight * 0.5f + 58.0f;
        glColor3f(0.08f, 0.10f, 0.14f);
        glBegin(GL_QUADS);
        glVertex2f(panelLeft, panelTop);
        glVertex2f(panelRight, panelTop);
        glVertex2f(panelRight, panelBottom);
        glVertex2f(panelLeft, panelBottom);
        glEnd();

        glColor3f(0.9f, 0.93f, 0.97f);
        drawCenteredText(Settings::ScreenHeight * 0.5f - 12.0f, "GAME OVER - Press R");
        drawCenteredText(Settings::ScreenHeight * 0.5f + 14.0f, scoreText);
        drawCenteredText(Settings::ScreenHeight * 0.5f + 40.0f, highScoreText);
    }
    else if (ballAttached)
    {
        drawCenteredText(34.0f, "Press SPACE to start");
    }
    else if (!activeEffectNotices.empty())
    {
        for (size_t i = 0; i < activeEffectNotices.size(); ++i)
        {
            char text[96];
            const ActiveEffectNotice& notice = activeEffectNotices[i];
            std::snprintf(text, sizeof(text), "%s %.1fs", notice.text.c_str(), notice.timer);
            drawCenteredNotice(34.0f + static_cast<float>(i) * 26.0f, text);
        }
    }
}
