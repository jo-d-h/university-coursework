#include "Collision.h"
#include <algorithm>
#include "../objects/Ball.h"

namespace Collision
{
// 입사 벡터를 법선 기준으로 반사
Vec2 reflect(const Vec2& velocity, const Vec2& normal)
{
    return velocity - normal * (2.0f * dot(velocity, normal));
}

// 공 중심 - 선분의 가장 가까운 점 거리 기준으로 충돌 판정
bool resolveCircleSegment(Ball& ball, const Vec2& start, const Vec2& end)
{
    const Vec2 segment = end - start;
    const float segmentLengthSquared = segment.lengthSquared();
    if (segmentLengthSquared <= 0.0001f)
    {
        return false;
    }

    float t = dot(ball.position - start, segment) / segmentLengthSquared;
    t = std::max(0.0f, std::min(1.0f, t));

    const Vec2 closest = start + segment * t;
    const Vec2 offset = ball.position - closest;
    const float distanceSquared = offset.lengthSquared();
    if (distanceSquared > ball.radius * ball.radius)
    {
        return false;
    }

    Vec2 normal;
    if (distanceSquared <= 0.0001f)
    {
        normal = normalize(Vec2(-segment.y, segment.x));
    }
    else
    {
        normal = normalize(offset);
    }

    if (dot(ball.velocity, normal) < 0.0f)
    {
        ball.velocity = reflect(ball.velocity, normal);
    }

    const float penetration = ball.radius - offset.length();
    ball.position += normal * (penetration + 0.5f);
    return true;
}

// 공 중심 - 사각형의 가장 가까운 점으로 충돌판정
bool resolveCircleRect(Ball& ball, const Vec2& center, float width, float height)
{
    const float left = center.x - width * 0.5f;
    const float right = center.x + width * 0.5f;
    const float top = center.y - height * 0.5f;
    const float bottom = center.y + height * 0.5f;

    const Vec2 closest(
        std::max(left, std::min(right, ball.position.x)),
        std::max(top, std::min(bottom, ball.position.y)));

    const Vec2 offset = ball.position - closest;
    if (offset.lengthSquared() > ball.radius * ball.radius)
    {
        return false;
    }

    Vec2 normal;
    if (std::abs(offset.x) > std::abs(offset.y))
    {
        normal = Vec2(offset.x > 0.0f ? 1.0f : -1.0f, 0.0f);
    }
    else
    {
        normal = Vec2(0.0f, offset.y > 0.0f ? 1.0f : -1.0f);
    }

    if (dot(ball.velocity, normal) < 0.0f)
    {
        ball.velocity = reflect(ball.velocity, normal);
    }
    ball.position += normal * 2.0f;
    return true;
}
}
