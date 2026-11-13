#pragma once

#include "../core/Vec2.h"

class Ball;

namespace Collision
{
    // 법선 벡터 기준 반사 벡터 계산
    Vec2 reflect(const Vec2& velocity, const Vec2& normal);

    // 원과 선분 충돌 처리
    bool resolveCircleSegment(Ball& ball, const Vec2& start, const Vec2& end);

    // 원과 사각형 충돌 처리
    bool resolveCircleRect(Ball& ball, const Vec2& center, float width, float height);
}
