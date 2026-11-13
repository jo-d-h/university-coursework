#include "Ball.h"

#include <cmath>
#include <windows.h>
#include <gl/gl.h>

#include "../core/Settings.h"

Ball::Ball(const Vec2& startPosition)
    : position(startPosition), velocity(0.0f, 0.0f), radius(Settings::BallRadius)
{
}

// 공 최초 발사
void Ball::launch()
{
    velocity = Vec2(Settings::BallLaunchXSpeed, Settings::BallStartSpeed);
}

// 공 위치 업데이트
void Ball::update(float dt)
{
    position += velocity * dt;
}

// 공 출력(원 형태)
void Ball::draw(float red, float green, float blue) const
{
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y);
    for (int i = 0; i <= 32; ++i)
    {
        const float angle = static_cast<float>(i) / 32.0f * 6.2831853f;
        glVertex2f(position.x + std::cos(angle) * radius, position.y + std::sin(angle) * radius);
    }
    glEnd();
}
