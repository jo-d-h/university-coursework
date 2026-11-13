#include "Item.h"

#include <cmath>
#include <windows.h>
#include <gl/gl.h>

Item::Item(const Vec2& startPosition, ItemType itemType)
    : position(startPosition), type(itemType), velocity(0.0f, -100.0f)
{
}

// 아이템 상승 및 회전선 도달시 삭제
void Item::update(float dt)
{
    position += velocity * dt;
    life -= dt;
    if (life <= 0.0f || position.y < 40.0f)
    {
        expired = true;
    }
}

// 각 특수효과별 아이콘
void Item::drawIcon(ItemType type, const Vec2& center, float scale)
{
    glColor3f(0.08f, 0.10f, 0.14f);
    glLineWidth(2.0f);
    switch (type)
    {
    case ItemType::ExtraBall:
        glBegin(GL_LINES);
        glVertex2f(center.x, center.y - 8.0f * scale);
        glVertex2f(center.x, center.y + 8.0f * scale);
        glVertex2f(center.x - 8.0f * scale, center.y);
        glVertex2f(center.x + 8.0f * scale, center.y);
        glEnd();
        break;
    case ItemType::ExpandPaddle:
        glBegin(GL_LINES);
        glVertex2f(center.x - 8.5f * scale, center.y);
        glVertex2f(center.x + 8.5f * scale, center.y);
        glVertex2f(center.x - 8.5f * scale, center.y);
        glVertex2f(center.x - 4.5f * scale, center.y - 4.0f * scale);
        glVertex2f(center.x - 8.5f * scale, center.y);
        glVertex2f(center.x - 4.5f * scale, center.y + 4.0f * scale);
        glVertex2f(center.x + 8.5f * scale, center.y);
        glVertex2f(center.x + 4.5f * scale, center.y - 4.0f * scale);
        glVertex2f(center.x + 8.5f * scale, center.y);
        glVertex2f(center.x + 4.5f * scale, center.y + 4.0f * scale);
        glEnd();
        break;
    case ItemType::SpeedUp:
        glBegin(GL_LINE_LOOP);
        glVertex2f(center.x + 1.5f * scale, center.y - 8.0f * scale);
        glVertex2f(center.x - 5.5f * scale, center.y + 0.5f * scale);
        glVertex2f(center.x - 1.0f * scale, center.y + 0.5f * scale);
        glVertex2f(center.x - 3.0f * scale, center.y + 8.0f * scale);
        glVertex2f(center.x + 6.0f * scale, center.y - 2.0f * scale);
        glVertex2f(center.x + 1.5f * scale, center.y - 2.0f * scale);
        glEnd();
        break;
    case ItemType::StopObstacle:
        glBegin(GL_LINES);
        glVertex2f(center.x - 4.0f * scale, center.y - 7.0f * scale);
        glVertex2f(center.x - 4.0f * scale, center.y + 7.0f * scale);
        glVertex2f(center.x + 4.0f * scale, center.y - 7.0f * scale);
        glVertex2f(center.x + 4.0f * scale, center.y + 7.0f * scale);
        glEnd();
        break;
    }
    glLineWidth(1.0f);
}

// 아이템 박스, 내부 아이콘
void Item::draw() const
{
    switch (type)
    {
    case ItemType::ExtraBall:
        glColor3f(0.47f, 0.92f, 1.0f);
        break;
    case ItemType::ExpandPaddle:
        glColor3f(1.0f, 0.84f, 0.35f);
        break;
    case ItemType::SpeedUp:
        glColor3f(1.0f, 0.36f, 0.40f);
        break;
    case ItemType::StopObstacle:
        glColor3f(0.65f, 1.0f, 0.72f);
        break;
    }
    const float half = Size * 0.5f;
    glBegin(GL_QUADS);
    glVertex2f(position.x - half, position.y - half);
    glVertex2f(position.x + half, position.y - half);
    glVertex2f(position.x + half, position.y + half);
    glVertex2f(position.x - half, position.y + half);
    glEnd();

    drawIcon(type, position);
}
