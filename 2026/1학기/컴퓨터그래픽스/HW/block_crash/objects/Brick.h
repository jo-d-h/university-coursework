#pragma once

#include "../core/Vec2.h"
#include "Item.h"

class Ball;

class Brick
{
public:
    Brick(const Vec2& startPosition, int colorIndex, bool isSpecial, ItemType itemType, float brickWidth);

    // 공과 벽돌 충돌 처리
    bool resolveBallCollision(Ball& ball);

    // 벽돌 위쪽으로 이동
    void moveUp(float amount);

    // 벽돌의 위쪽 y좌표 반환
    float top() const;

    // 벽돌 그리기
    void draw() const;

    Vec2 position;
    bool destroyed = false;
    bool special = false;
    ItemType specialType = ItemType::ExtraBall;

private:
    float width = 0.0f;
    int colorIndex = 0;
};
