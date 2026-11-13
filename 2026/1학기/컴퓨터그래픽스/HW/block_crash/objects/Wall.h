#pragma once

#include <array>

#include "../core/Vec2.h"

class Ball;

class Wall
{
public:
    Wall();

    // 공과 벽 충돌 처리
    void resolveBallCollision(Ball& ball) const;

    // 공이 제한선 벗어났는지 확인
    bool isOutOfBounds(const Ball& ball) const;

    // y좌표에서 왼쪽/오른쪽 벽 위치 계산
    float leftLimitAtY(float y) const;
    float rightLimitAtY(float y) const;

    // 벽 그리기
    void draw(float red, float green, float blue) const;

private:
    // 선분 위 특정 y좌표의 x값 계산
    float xOnSegment(const Vec2& a, const Vec2& b, float y) const;

    std::array<Vec2, 4> points;
};
