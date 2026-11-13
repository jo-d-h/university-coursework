#include "Game.h"

#include <algorithm>
#include <string>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

#include "Settings.h"
#include "../physics/Collision.h"
#include "../physics/Gravity.h"

namespace
{
// 공 속도를 최소/최대 범위로 보정
void keepBallMoving(Ball& ball)
{
    const float speed = ball.velocity.length();
    if (speed <= 0.0001f)
    {
        return;
    }

    if (speed < Settings::BallMinimumSpeed)
    {
        ball.velocity = ball.velocity * (Settings::BallMinimumSpeed / speed);
    }
    else if (speed > Settings::BallMaximumSpeed)
    {
        ball.velocity = ball.velocity * (Settings::BallMaximumSpeed / speed);
    }
}

}

// 게임 객체 초기화
void Game::initialize()
{
    reset();
}

// 점수와 오브젝트 상태 초기화
void Game::reset()
{
    score = 0;
    gameOver = false;
    ballAttached = true;
    activeEffectNotices.clear();
    nextRainbowScore = 2000;
    rainbowTimer = 0.0f;
    rainbowPhase = 0.0f;
    paddleRainbowTimer = 0.0f;
    obstacleRainbowTimer = 0.0f;
    brickAddTimer = Settings::BrickAddSeconds;
    paddle = Paddle(Vec2(Settings::ScreenWidth * 0.5f, Settings::PaddleStartY));
    obstacle = Obstacle(Vec2(Settings::ScreenWidth * 0.5f, Settings::ScreenHeight * 0.5f));
    balls.clear();
    balls.emplace_back(paddle.ballAnchor());
    items.clear();
    brickField.reset(bricks, wall);
}


// 매 프레임 게임 진행 처리
void Game::update(float dt)
{
    if (gameOver)
    {
        return;
    }

    updateInput(dt);
    paddle.update(dt);
    rainbowPhase += dt * 5.5f;
    if (rainbowTimer > 0.0f)
    {
        rainbowTimer = std::max(0.0f, rainbowTimer - dt);
    }
    paddleRainbowTimer = std::max(0.0f, paddleRainbowTimer - dt);
    obstacleRainbowTimer = std::max(0.0f, obstacleRainbowTimer - dt);
    if (!ballAttached)
    {
        obstacle.update(dt);
        updateBricks(dt);
        if (gameOver)
        {
            return;
        }

        brickAddTimer -= dt;
        if (brickAddTimer <= 0.0f)
        {
            brickField.addTimedRow(bricks, wall);
            brickAddTimer += Settings::BrickAddSeconds;
        }
    }

    if (ballAttached)
    {
        balls[0].position = paddle.ballAnchor();
    }
    else
    {
        for (Ball& ball : balls)
        {
            ball.rainbowTimer = std::max(0.0f, ball.rainbowTimer - dt);
            applyUpwardGravity(ball, dt);
            ball.update(dt);
            wall.resolveBallCollision(ball);
            paddle.resolveBallCollision(ball);
            obstacle.resolveBallCollision(ball);
            keepBallMoving(ball);

            for (Brick& brick : bricks)
            {
                if (!brick.destroyed && brick.resolveBallCollision(ball))
                {
                    keepBallMoving(ball);
                    score += 100;
                    highScore = std::max(highScore, score);
                    if (score >= nextRainbowScore)
                    {
                        rainbowTimer = Settings::RainbowSeconds;
                        showEffectNotice("Over " + std::to_string(nextRainbowScore) + " Points", Settings::RainbowSeconds);
                        while (nextRainbowScore <= score)
                        {
                            nextRainbowScore += 2000;
                        }
                    }
                    if (brick.special)
                    {
                        spawnSpecialItem(brick.position, brick.specialType);
                    }
                    break;
                }
            }
        }

        balls.erase(
            std::remove_if(balls.begin(), balls.end(), [this](const Ball& ball) {
                return wall.isOutOfBounds(ball);
            }),
            balls.end());

        if (balls.empty())
        {
            gameOver = true;
        }
    }

    for (Item& item : items)
    {
        item.update(dt);

        // 빨간 제한선 위로 올라가면 삭제
        if (item.position.y <= Settings::LimitLineY)
        {
            item.expired = true;
        }

        if (!item.expired &&
            paddle.intersectsRect(item.position, Item::Size, Item::Size))
        {
            activateSpecialEffect(item.type, item.position);
            item.expired = true;
        }
    }

    for (ActiveEffectNotice& notice : activeEffectNotices)
    {
        notice.timer = std::max(0.0f, notice.timer - dt);
    }
    activeEffectNotices.erase(
        std::remove_if(activeEffectNotices.begin(), activeEffectNotices.end(), [](const ActiveEffectNotice& notice) {
            return notice.timer <= 0.0f;
        }),
        activeEffectNotices.end());

    bricks.erase(
        std::remove_if(bricks.begin(), bricks.end(), [](const Brick& brick) {
            return brick.destroyed;
        }),
        bricks.end());

    items.erase(
        std::remove_if(items.begin(), items.end(), [](const Item& item) {
            return item.expired;
        }),
        items.end());

    if (bricks.empty())
    {
        brickField.reset(bricks, wall);
        brickAddTimer = Settings::BrickAddSeconds;
    }
}

//  패들 이동 처리
void Game::updateInput(float dt)
{
    float direction = 0.0f;
    if (leftPressed)
    {
        direction -= 1.0f;
    }
    if (rightPressed)
    {
        direction += 1.0f;
    }
    paddle.move(direction, dt, wall);
}

// 벽돌 상승, 제한선 도달 검사
void Game::updateBricks(float dt)
{
    for (Brick& brick : bricks)
    {
        brick.moveUp(Settings::BrickRiseSpeed * dt);
        if (!brick.destroyed && brick.top() <= Settings::LimitLineY)
        {
            gameOver = true;
        }
    }
}

// 특수 벽돌 제거 시 아이템 생성
void Game::spawnSpecialItem(const Vec2& position, ItemType type)
{
    items.emplace_back(position, type);
}

// 아이템에 따라 특수효과 적용
void Game::activateSpecialEffect(ItemType type, const Vec2& position)
{
    switch (type)
    {
    case ItemType::ExtraBall:
        if (!balls.empty() && static_cast<int>(balls.size()) < Settings::MaxBalls)
        {
            Ball extra(position);
            extra.velocity = Vec2(-balls[0].velocity.x * 0.8f + 80.0f, balls[0].velocity.y);
            extra.rainbowTimer = 3.0f;
            balls.push_back(extra);
        }
        showEffectNotice("Extra Ball", 3.0f);
        break;
    case ItemType::ExpandPaddle:
        paddle.expand();
        paddleRainbowTimer = Settings::PaddleExpandSeconds;
        showEffectNotice("Paddle Expand", Settings::PaddleExpandSeconds);
        break;
    case ItemType::SpeedUp:
        for (Ball& ball : balls)
        {
            ball.velocity = ball.velocity * Settings::SpeedUpMultiplier;
            keepBallMoving(ball);
            ball.rainbowTimer = 3.0f;
        }
        showEffectNotice("Speed Up", 3.0f);
        break;
    case ItemType::StopObstacle:
        obstacle.stopTemporarily(Settings::ObstacleStopSeconds);
        obstacleRainbowTimer = Settings::ObstacleStopSeconds;
        showEffectNotice("Obstacle Stop", Settings::ObstacleStopSeconds);
        break;
    }
}

// HUD에 효과 지속시간 표시
void Game::showEffectNotice(const std::string& text, float seconds)
{
    for (ActiveEffectNotice& notice : activeEffectNotices)
    {
        if (notice.text == text)
        {
            notice.timer = std::max(notice.timer, seconds);
            return;
        }
    }

    activeEffectNotices.push_back({text, seconds});
}


// 게임 화면 출력
void Game::render() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    const Color world = Effects::worldColor(rainbowTimer, rainbowPhase);
    wall.draw(world.red, world.green, world.blue);

    glColor3f(1.0f, 0.28f, 0.34f);
    glBegin(GL_LINES);
    glVertex2f(wall.leftLimitAtY(Settings::LimitLineY), Settings::LimitLineY);
    glVertex2f(wall.rightLimitAtY(Settings::LimitLineY), Settings::LimitLineY);
    glEnd();

    for (const Brick& brick : bricks)
    {
        brick.draw();
    }
    for (const Item& item : items)
    {
        item.draw();
    }
    Color obstacleColor = world;
    if (obstacleRainbowTimer > 0.0f)
    {
        obstacleColor = Effects::rainbowColor(rainbowPhase);
    }
    obstacle.draw(obstacleColor.red, obstacleColor.green, obstacleColor.blue);

    Color paddleColor{0.96f, 0.76f, 0.32f};
    if (paddleRainbowTimer > 0.0f)
    {
        paddleColor = Effects::rainbowColor(rainbowPhase);
    }
    paddle.draw(paddleColor.red, paddleColor.green, paddleColor.blue);

    for (const Ball& ball : balls)
    {
        Color ballColor{0.93f, 0.96f, 1.0f};
        if (ball.rainbowTimer > 0.0f)
        {
            ballColor = Effects::rainbowColor(rainbowPhase);
        }
        ball.draw(ballColor.red, ballColor.green, ballColor.blue);
    }

    drawUi();
    glutSwapBuffers();
}

// HUD 호출
void Game::drawUi() const
{
    Hud::draw(score, highScore, gameOver, ballAttached, activeEffectNotices);
}


// 키를 눌렀을 때 입력 상태 변경
void Game::onKeyDown(unsigned char key)
{
    if (key == 'a' || key == 'A')
    {
        leftPressed = true;
    }
    else if (key == 'd' || key == 'D')
    {
        rightPressed = true;
    }
    else if (key == ' ' && ballAttached && !gameOver)
    {
        ballAttached = false;
        balls[0].launch();
    }
    else if ((key == 'r' || key == 'R') && gameOver)
    {
        reset();
    }
}

// 키를 뗐을 때 입력 상태 변경
void Game::onKeyUp(unsigned char key)
{
    if (key == 'a' || key == 'A')
    {
        leftPressed = false;
    }
    else if (key == 'd' || key == 'D')
    {
        rightPressed = false;
    }
}

// 방향키 입력 처리
void Game::onSpecialKeyDown(int key)
{
    if (key == GLUT_KEY_LEFT)
    {
        leftPressed = true;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        rightPressed = true;
    }
}

// 방향키 해제 처리
void Game::onSpecialKeyUp(int key)
{
    if (key == GLUT_KEY_LEFT)
    {
        leftPressed = false;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        rightPressed = false;
    }
}
