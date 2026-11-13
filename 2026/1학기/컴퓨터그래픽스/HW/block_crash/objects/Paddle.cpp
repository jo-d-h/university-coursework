#include "Paddle.h"

#include <algorithm>
#include <cmath>
#include <windows.h>
#include <gl/gl.h>

#include "Ball.h"
#include "Wall.h"
#include "../core/Settings.h"

Paddle::Paddle(const Vec2& startPosition)
    : position(startPosition), width(Settings::PaddleWidth)
{
}

// 시작 전 공이 패들 아래에 붙어 있도록
Vec2 Paddle::ballAnchor() const
{
    return Vec2(position.x, position.y + Settings::PaddleHeight * 0.5f + 14.0f);
}

// 벽 범위 안에서 좌우 이동
void Paddle::move(float direction, float dt, const Wall& wall)
{
    position.x += direction * Settings::PaddleSpeed * dt;
    const float left = wall.leftLimitAtY(position.y) + width * 0.5f;
    const float right = wall.rightLimitAtY(position.y) - width * 0.5f;
    position.x = std::max(left, std::min(right, position.x));
}

// 확장 효과 시간이 끝나면 원위치
void Paddle::update(float dt)
{
    if (expandTimer > 0.0f)
    {
        expandTimer -= dt;
        if (expandTimer <= 0.0f)
        {
            width = Settings::PaddleWidth;
        }
    }
}

// 패들 확장 아이템 효과
void Paddle::expand()
{
    width = Settings::PaddleExpandWidth;
    expandTimer = Settings::PaddleExpandSeconds;
}

// 아이템 획득 여부 확인
bool Paddle::intersectsRect(const Vec2& center, float rectWidth, float rectHeight) const
{
    const float paddleLeft = position.x - width * 0.5f;
    const float paddleRight = position.x + width * 0.5f;
    const float paddleTop = position.y - Settings::PaddleHeight * 0.5f;
    const float paddleBottom = position.y + Settings::PaddleHeight * 0.5f;
    const float rectLeft = center.x - rectWidth * 0.5f;
    const float rectRight = center.x + rectWidth * 0.5f;
    const float rectTop = center.y - rectHeight * 0.5f;
    const float rectBottom = center.y + rectHeight * 0.5f;

    return rectRight >= paddleLeft &&
           rectLeft <= paddleRight &&
           rectBottom >= paddleTop &&
           rectTop <= paddleBottom;
}

// 충돌 위치에 따라 반사 각도 조절
bool Paddle::resolveBallCollision(Ball& ball) const
{
    const float left = position.x - width * 0.5f;
    const float right = position.x + width * 0.5f;
    const float top = position.y - Settings::PaddleHeight * 0.5f;
    const float bottom = position.y + Settings::PaddleHeight * 0.5f;

    if (ball.velocity.y >= 0.0f)
    {
        return false;
    }
    if (ball.position.x < left || ball.position.x > right)
    {
        return false;
    }
    if (ball.position.y - ball.radius > bottom || ball.position.y + ball.radius < top)
    {
        return false;
    }

    const float offset = std::max(-1.0f, std::min(1.0f, (ball.position.x - position.x) / (width * 0.5f)));
    float speed = ball.velocity.length();
    speed = std::max(Settings::BallMinimumSpeed, std::min(Settings::BallMaximumSpeed, speed));

    const float maxAngle = Settings::PaddleMaxBounceAngleDegrees * 3.14159265f / 180.0f;
    const float angle = offset * maxAngle;

    ball.position.y = bottom + ball.radius;
    ball.velocity = Vec2(std::sin(angle) * speed, std::cos(angle) * speed);
    return true;
}

// 사각형 패들 출력
void Paddle::draw(float red, float green, float blue) const
{
    const float left = position.x - width * 0.5f;
    const float right = position.x + width * 0.5f;
    const float top = position.y - Settings::PaddleHeight * 0.5f;
    const float bottom = position.y + Settings::PaddleHeight * 0.5f;

    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();
}
