#pragma once

#include "../core/Vec2.h"

class Ball;

class Obstacle
{
public:
    Obstacle() = default;
    explicit Obstacle(const Vec2& startPosition);

    // 회전선 각도 갱신
    void update(float dt);

    // 회전 일시정지
    void stopTemporarily(float seconds);

    // 공과 회전선 충돌 처리
    bool resolveBallCollision(Ball& ball) const;

    // 회전선 그리기
    void draw(float red, float green, float blue) const;

private:
    // 회전선 양 끝점 계산
    void endpoints(Vec2& start, Vec2& end) const;

    Vec2 position;
    float length = 160.0f;
    float angleDegrees = 0.0f;
    float angularSpeed = 47.5f;
    float stopTimer = 0.0f;
};
