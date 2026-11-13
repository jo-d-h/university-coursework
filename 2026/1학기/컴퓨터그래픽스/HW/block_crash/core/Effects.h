#pragma once

struct Color
{
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
};

namespace Effects
{
    // 벽 색상 또는 무지개 색상 계산
    Color worldColor(float rainbowTimer, float rainbowPhase);

    // 시간값으로 무지개 색상 계산
    Color rainbowColor(float rainbowPhase);
}
