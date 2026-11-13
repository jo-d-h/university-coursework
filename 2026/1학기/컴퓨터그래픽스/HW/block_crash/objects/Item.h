#pragma once

#include "../core/Vec2.h"

enum class ItemType
{
    ExtraBall,
    ExpandPaddle,
    SpeedUp,
    StopObstacle
};

class Item
{
public:
    Item(const Vec2& startPosition, ItemType itemType);

    // 아이템 이동, 제한선 도달 처리
    void update(float dt);

    // 아이템 그리기
    void draw() const;

    // 벽돌 내부 특수효과 아이콘
    static void drawIcon(ItemType type, const Vec2& center, float scale = 1.0f);

    static constexpr float Size = 16.0f;

    Vec2 position;
    ItemType type;
    bool expired = false;

private:
    Vec2 velocity;
    float life = 5.0f;
};
