#pragma once

#include "../core/Vec2.h"

class Ball
{
public:
    explicit Ball(const Vec2& startPosition);

    // 공 발사
    void launch();

    // 속도에 따라 위치 갱신
    void update(float dt);

    // 공 그리기
    void draw(float red, float green, float blue) const;

    Vec2 position;
    Vec2 velocity;
    float radius;
    float rainbowTimer = 0.0f;
};
