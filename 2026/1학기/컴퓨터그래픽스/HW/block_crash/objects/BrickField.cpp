#include "BrickField.h"

#include <algorithm>
#include <array>
#include <random>
#include <vector>

#include "Wall.h"
#include "../core/Settings.h"

namespace
{
struct BrickSlot
{
    Vec2 position;
    int row = 0;
    int col = 0;
};

// 특수효과 블록 랜덤 선정
std::mt19937& randomEngine()
{
    static std::mt19937 engine(std::random_device{}());
    return engine;
}
}

// 초기 벽돌 배치, 특수효과 블록 선정
void BrickField::reset(std::vector<Brick>& bricks, const Wall& wall)
{
    bricks.clear();
    std::vector<BrickSlot> slots;
    const float pitch = Settings::BrickWidth + Settings::BrickColumnGap;
    const float gridWidth = Settings::BrickColumns * Settings::BrickWidth +
                            (Settings::BrickColumns - 1) * Settings::BrickColumnGap;
    const float gridLeft = (Settings::ScreenWidth - gridWidth) * 0.5f;

    for (int row = 0; row < Settings::BrickRows; ++row)
    {
        const float y = Settings::BrickStartY + row * (Settings::BrickHeight + Settings::BrickRowGap);
        const float bottom = y + Settings::BrickHeight * 0.5f;
        const float leftEdge = wall.leftLimitAtY(bottom) + Settings::BrickSidePadding;
        const float rightEdge = wall.rightLimitAtY(bottom) - Settings::BrickSidePadding;

        for (int col = 0; col < Settings::BrickColumns; ++col)
        {
            const float brickLeft = gridLeft + col * pitch;
            const float brickRight = brickLeft + Settings::BrickWidth;
            if (brickLeft < leftEdge || brickRight > rightEdge)
            {
                continue;
            }

            slots.push_back({
                Vec2(brickLeft + Settings::BrickWidth * 0.5f, y),
                row,
                col,
            });
        }
    }

    std::vector<int> specialIndexes;
    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        const BrickSlot& slot = slots[i];
        bool unreachableTopEdge = false;
        if (slot.row < 2)
        {
            int rowMinCol = Settings::BrickColumns;
            int rowMaxCol = -1;
            for (const BrickSlot& other : slots)
            {
                if (other.row == slot.row)
                {
                    rowMinCol = std::min(rowMinCol, other.col);
                    rowMaxCol = std::max(rowMaxCol, other.col);
                }
            }
            unreachableTopEdge = slot.col == rowMinCol || slot.col == rowMaxCol;
        }
        if (!unreachableTopEdge)
        {
            specialIndexes.push_back(i);
        }
    }
    std::shuffle(specialIndexes.begin(), specialIndexes.end(), randomEngine());

    std::vector<ItemType> specialTypes(slots.size(), ItemType::ExtraBall);
    std::vector<bool> isSpecial(slots.size(), false);
    const std::array<ItemType, 4> requiredSpecials = {
        ItemType::ExtraBall,
        ItemType::ExpandPaddle,
        ItemType::SpeedUp,
        ItemType::StopObstacle,
    };

    const int specialCount = std::min(static_cast<int>(requiredSpecials.size()), static_cast<int>(specialIndexes.size()));
    for (int i = 0; i < specialCount; ++i)
    {
        const int slotIndex = specialIndexes[i];
        isSpecial[slotIndex] = true;
        specialTypes[slotIndex] = requiredSpecials[i];
    }

    for (size_t i = 0; i < slots.size(); ++i)
    {
        bricks.emplace_back(slots[i].position, slots[i].row, isSpecial[i], specialTypes[i], Settings::BrickWidth);
    }
    nextRowColor = Settings::BrickRows;
}

// 시간 경과 시 벽돌 추가 생성(최하단 1줄), 특수효과 랜덤 선정
void BrickField::addTimedRow(std::vector<Brick>& bricks, const Wall& wall)
{
    const float bottomRowY = Settings::BrickStartY +
        (Settings::BrickRows - 1) * (Settings::BrickHeight + Settings::BrickRowGap);
    addRow(bricks, wall, bottomRowY, nextRowColor++, true);
}

// 벽 내부에서만 생성되도록 계산
void BrickField::addRow(std::vector<Brick>& bricks, const Wall& wall, float y, int colorIndex, bool includeRandomSpecial)
{
    const float pitch = Settings::BrickWidth + Settings::BrickColumnGap;
    const float gridWidth = Settings::BrickColumns * Settings::BrickWidth +
                            (Settings::BrickColumns - 1) * Settings::BrickColumnGap;
    const float gridLeft = (Settings::ScreenWidth - gridWidth) * 0.5f;
    const float bottom = y + Settings::BrickHeight * 0.5f;
    const float leftEdge = wall.leftLimitAtY(bottom) + Settings::BrickSidePadding;
    const float rightEdge = wall.rightLimitAtY(bottom) - Settings::BrickSidePadding;
    std::vector<Vec2> positions;

    for (int col = 0; col < Settings::BrickColumns; ++col)
    {
        const float brickLeft = gridLeft + col * pitch;
        const float brickRight = brickLeft + Settings::BrickWidth;
        if (brickLeft < leftEdge || brickRight > rightEdge)
        {
            continue;
        }

        positions.emplace_back(brickLeft + Settings::BrickWidth * 0.5f, y);
    }

    int specialIndex = -1;
    ItemType specialType = ItemType::ExtraBall;
    if (includeRandomSpecial && !positions.empty())
    {
        std::uniform_int_distribution<int> positionDist(0, static_cast<int>(positions.size()) - 1);
        std::uniform_int_distribution<int> typeDist(0, 3);
        specialIndex = positionDist(randomEngine());
        specialType = static_cast<ItemType>(typeDist(randomEngine()));
    }

    for (int i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        const bool special = i == specialIndex;
        bricks.emplace_back(positions[i], colorIndex, special, specialType, Settings::BrickWidth);
    }
}
