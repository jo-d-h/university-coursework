#pragma once

namespace Settings
{
    // 화면 및 프레임 설정
    constexpr int ScreenWidth = 900;
    constexpr int ScreenHeight = 900;
    constexpr int FrameMilliseconds = 16;
    constexpr float FixedDeltaTime = FrameMilliseconds / 1000.0f;

    // 공 물리 설정
    constexpr float UpwardGravity = 22.0f;
    constexpr float BallRadius = 9.0f;
    constexpr float BallLaunchXSpeed = 112.5f;
    constexpr float BallStartSpeed = 322.5f;
    constexpr float BallMinimumSpeed = 300.0f;
    constexpr float BallMaximumSpeed = 520.0f;
    constexpr int MaxBalls = 4;

    // 패들 설정
    constexpr float PaddleStartY = 245.0f;
    constexpr float PaddleWidth = 85.0f;
    constexpr float PaddleHeight = 18.0f;
    constexpr float PaddleSpeed = 520.0f;
    constexpr float PaddleExpandWidth = 145.0f;
    constexpr float PaddleExpandSeconds = 7.0f;
    constexpr float PaddleMaxBounceAngleDegrees = 62.0f;

    // 벽돌 및 제한선 설정
    constexpr float BrickStartY = 560.0f;
    constexpr float BrickWidth = 44.0f;
    constexpr float BrickHeight = 22.0f;
    constexpr float BrickColumnGap = 4.0f;
    constexpr float BrickRowGap = 2.0f;
    constexpr float BrickSidePadding = 0.0f;
    constexpr int BrickRows = 4;
    constexpr int BrickColumns = 9;
    constexpr float BrickRiseSpeed = 4.0f;
    constexpr float BrickAddSeconds = (BrickHeight + BrickRowGap) / BrickRiseSpeed;
    constexpr float LimitLineY = PaddleStartY;
    constexpr float RainbowSeconds = 3.0f;

    // 특수효과 설정
    constexpr float ObstacleStopSeconds = 4.0f;
    constexpr float SpeedUpMultiplier = 1.18f;
}
