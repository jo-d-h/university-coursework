#include "Gravity.h"

#include "../core/Settings.h"
#include "../objects/Ball.h"

// y속도 감소, 위쪽으로 가속
void applyUpwardGravity(Ball& ball, float dt)
{
    ball.velocity.y -= Settings::UpwardGravity * dt;
}
