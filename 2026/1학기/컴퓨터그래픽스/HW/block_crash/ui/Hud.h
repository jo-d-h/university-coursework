#pragma once

#include <string>
#include <vector>

struct ActiveEffectNotice
{
    std::string text;
    float timer = 0.0f;
};

namespace Hud
{
    // 점수, 안내 문구, 게임오버 출력
    void draw(
        int score,
        int highScore,
        bool gameOver,
        bool ballAttached,
        const std::vector<ActiveEffectNotice>& activeEffectNotices);
}
