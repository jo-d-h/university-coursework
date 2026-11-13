#include "Obstacle.h"

#include <cmath>
#include <windows.h>
#include <gl/gl.h>

#include "Ball.h"
#include "../physics/Collision.h"

Obstacle::Obstacle(const Vec2& startPosition) : position(startPosition)
{
}

// 정지 타이머가 작동중이지 않을때만 업데이트하여 회전
void Obstacle::update(float dt)
{
    if (stopTimer > 0.0f)
    {
        stopTimer -= dt;
        return;
    }
    angleDegrees += angularSpeed * dt;
}

// 특수효과 회전 일시 정지
void Obstacle::stopTemporarily(float seconds)
{
    stopTimer = seconds;
}

// 회전선과 공 충돌 처리
bool Obstacle::resolveBallCollision(Ball& ball) const
{
    Vec2 start;
    Vec2 end;
    endpoints(start, end);
    return Collision::resolveCircleSegment(ball, start, end);
}

// 각도에 따라 회전선 좌표 계산
void Obstacle::endpoints(Vec2& start, Vec2& end) const
{
    const float radians = angleDegrees * 3.14159265f / 180.0f;
    const Vec2 half(std::cos(radians) * length * 0.5f, std::sin(radians) * length * 0.5f);
    start = position - half;
    end = position + half;
}

// 회전선 장애물 화면출력
void Obstacle::draw(float red, float green, float blue) const
{
    Vec2 start;
    Vec2 end;
    endpoints(start, end);

    glColor3f(red, green, blue);
    const float radians = angleDegrees * 3.14159265f / 180.0f;

    const float dx = std::cos(radians);
    const float dy = std::sin(radians);

    const float halfLength = length * 0.5f;
    const float halfWidth = 7.0f;

    Vec2 p1(
        position.x - dx * halfLength - dy * halfWidth,
        position.y - dy * halfLength + dx * halfWidth
    );

    Vec2 p2(
        position.x + dx * halfLength - dy * halfWidth,
        position.y + dy * halfLength + dx * halfWidth
    );

    Vec2 p3(
        position.x + dx * halfLength + dy * halfWidth,
        position.y + dy * halfLength - dx * halfWidth
    );

    Vec2 p4(
        position.x - dx * halfLength + dy * halfWidth,
        position.y - dy * halfLength - dx * halfWidth
    );

    glColor3f(red, green, blue);

    glBegin(GL_QUADS);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glVertex2f(p4.x, p4.y);
    glEnd();

    constexpr int axisSegments = 24;
    constexpr float outerRadius = 11.0f;
    constexpr float innerRadius = 5.0f;
    glColor3f(1.0f, 1.0f, 1.0f);;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y);
    for (int i = 0; i <= axisSegments; ++i)
    {
        const float angle = static_cast<float>(i) * 2.0f * 3.14159265f / axisSegments;
        glVertex2f(position.x + std::cos(angle) * outerRadius, position.y + std::sin(angle) * outerRadius);
    }
    glEnd();

    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y);
    for (int i = 0; i <= axisSegments; ++i)
    {
        const float angle = static_cast<float>(i) * 2.0f * 3.14159265f / axisSegments;
        glVertex2f(position.x + std::cos(angle) * innerRadius, position.y + std::sin(angle) * innerRadius);
    }
    glEnd();
    glLineWidth(1.0f);
}
