#pragma once

#include <vector>

#include "Brick.h"

class Wall;

class BrickField
{
public:
    // 벽돌의 초기 배치
    void reset(std::vector<Brick>& bricks, const Wall& wall);

    // 일정 간격으로 새 벽돌 추가
    void addTimedRow(std::vector<Brick>& bricks, const Wall& wall);

private:
    // 벽돌 한 줄 생성
    void addRow(std::vector<Brick>& bricks, const Wall& wall, float y, int colorIndex, bool includeRandomSpecial);

    int nextRowColor = 0;
};
