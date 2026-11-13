#pragma once

#include <vector>

#include "Effects.h"
#include "../ui/Hud.h"
#include "../objects/Ball.h"
#include "../objects/Brick.h"
#include "../objects/BrickField.h"
#include "../objects/Item.h"
#include "../objects/Obstacle.h"
#include "../objects/Paddle.h"
#include "../objects/Wall.h"

class Game
{
public:
    // 게임 초기화
    void initialize();

    // 프레임마다 게임 업데이트
    void update(float dt);

    // 렌더링
    void render() const;

    // 키 입력
    void onKeyDown(unsigned char key);
    void onKeyUp(unsigned char key);
    void onSpecialKeyDown(int key);
    void onSpecialKeyUp(int key);

private:
    // 게임 초기화
    void reset();

    // 입력에 따라 패들 이동
    void updateInput(float dt);

    // 벽돌 상승, 제한선 판정
    void updateBricks(float dt);

    // 특수효과
    void spawnSpecialItem(const Vec2& position, ItemType type);
    void activateSpecialEffect(ItemType type, const Vec2& position);
    void showEffectNotice(const std::string& text, float seconds);

    // HUD 출력
    void drawUi() const;

    Wall wall;
    BrickField brickField;
    Paddle paddle;
    Obstacle obstacle;
    std::vector<Ball> balls;
    std::vector<Brick> bricks;
    std::vector<Item> items;

    bool leftPressed = false;
    bool rightPressed = false;
    bool ballAttached = true;
    bool gameOver = false;
    int score = 0;
    int highScore = 0;
    int nextRainbowScore = 2000;
    float rainbowTimer = 0.0f;
    float rainbowPhase = 0.0f;
    float paddleRainbowTimer = 0.0f;
    float obstacleRainbowTimer = 0.0f;
    float brickAddTimer = 0.0f;
    std::vector<ActiveEffectNotice> activeEffectNotices;
};
