#include "Wall.h"

#include <windows.h>
#include <gl/gl.h>

#include "Ball.h"
#include "../core/Settings.h"
#include "../physics/Collision.h"

Wall::Wall()
    : points{
          Vec2(Settings::ScreenWidth * 0.5f, 70.0f),
          Vec2(Settings::ScreenWidth - 90.0f, Settings::ScreenHeight * 0.5f),
          Vec2(Settings::ScreenWidth * 0.5f, Settings::ScreenHeight - 70.0f),
          Vec2(90.0f, Settings::ScreenHeight * 0.5f),
      }
{
}

// 기울어진 벽의 선분 충돌 검사
void Wall::resolveBallCollision(Ball& ball) const
{
    for (int i = 0; i < 4; ++i)
    {
        Collision::resolveCircleSegment(ball, points[i], points[(i + 1) % 4]);
    }
}

// 공이 제한선, 화면 밖으로 나갔는지 확인
bool Wall::isOutOfBounds(const Ball& ball) const
{
    return ball.position.y + ball.radius < Settings::LimitLineY ||
           ball.position.y > Settings::ScreenHeight + 80.0f;
}

// y좌표 기준 왼쪽 경계 계산
float Wall::leftLimitAtY(float y) const
{
    if (y <= Settings::ScreenHeight * 0.5f)
    {
        return xOnSegment(points[3], points[0], y);
    }
    return xOnSegment(points[3], points[2], y);
}

// y좌표 기준 오른쪽 경계 계산
float Wall::rightLimitAtY(float y) const
{
    if (y <= Settings::ScreenHeight * 0.5f)
    {
        return xOnSegment(points[0], points[1], y);
    }
    return xOnSegment(points[2], points[1], y);
}

// 보간 계산
float Wall::xOnSegment(const Vec2& a, const Vec2& b, float y) const
{
    const float t = (y - a.y) / (b.y - a.y);
    return a.x + (b.x - a.x) * t;
}

// 플레이 벽 출력
void Wall::draw(float red, float green, float blue) const
{
    glColor3f(red, green, blue);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
    for (const Vec2& point : points)
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
    glLineWidth(1.0f);
}
