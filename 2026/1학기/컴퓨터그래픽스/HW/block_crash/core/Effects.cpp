#include "Effects.h"

#include <cmath>

// 타이머 작동시에만 효과 적용
Color Effects::worldColor(float rainbowTimer, float rainbowPhase)
{
    if (rainbowTimer <= 0.0f)
    {
        return Color{0.38f, 0.86f, 0.75f};
    }

    return rainbowColor(rainbowPhase);
}

// RGB 값 변화
Color Effects::rainbowColor(float rainbowPhase)
{
    return Color{
        0.55f + 0.45f * std::sin(rainbowPhase),
        0.55f + 0.45f * std::sin(rainbowPhase + 2.0943951f),
        0.55f + 0.45f * std::sin(rainbowPhase + 4.1887902f),
    };
}
