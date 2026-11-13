#pragma once

#include "../core/Vec2.h"

class Ball;
class Wall;

class Paddle
{
public:
    Paddle() = default;
    explicit Paddle(const Vec2& startPosition);

    // 공 대기 위치 반환
    Vec2 ballAnchor() const;

    // 패들 확장 시간 업데이트
    void update(float dt);

    // 패들 이동
    void move(float direction, float dt, const Wall& wall);

    // 특수효과 패들 확장
    void expand();

    // 아이템과 패들 충돌 판정
    bool intersectsRect(const Vec2& center, float rectWidth, float rectHeight) const;

    // 공과 패들 충돌 처리
    bool resolveBallCollision(Ball& ball) const;

    // 패들 그리기
    void draw(float red, float green, float blue) const;

private:
    Vec2 position;
    float width = 0.0f;
    float expandTimer = 0.0f;
};
