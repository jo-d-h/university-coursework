#include "Brick.h"

#include <windows.h>
#include <gl/gl.h>

#include "../core/Settings.h"
#include "../physics/Collision.h"

Brick::Brick(const Vec2& startPosition, int rowColorIndex, bool isSpecial, ItemType itemType, float brickWidth)
    : position(startPosition), special(isSpecial), specialType(itemType), width(brickWidth), colorIndex(rowColorIndex)
{
}

// 공과 닿으면 벽돌 제거
bool Brick::resolveBallCollision(Ball& ball)
{
    if (Collision::resolveCircleRect(ball, position, width, Settings::BrickHeight))
    {
        destroyed = true;
        return true;
    }
    return false;
}

// 벽돌 지속 상승
void Brick::moveUp(float amount)
{
    position.y -= amount;
}

// 제한선 판정 좌표
float Brick::top() const
{
    return position.y - Settings::BrickHeight * 0.5f;
}

// 벽돌, 특수효과 아이콘 출력
void Brick::draw() const
{
    const float colors[][3] = {
        {0.91f, 0.36f, 0.46f},
        {0.31f, 0.65f, 0.91f},
        {0.49f, 0.80f, 0.43f},
        {0.94f, 0.60f, 0.33f},
    };
    const float* color = colors[colorIndex % 4];
    const float left = position.x - width * 0.5f;
    const float right = position.x + width * 0.5f;
    const float top = position.y - Settings::BrickHeight * 0.5f;
    const float bottom = position.y + Settings::BrickHeight * 0.5f;

    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();

    if (special)
    {
        Item::drawIcon(specialType, position, 0.75f);
    }
}
