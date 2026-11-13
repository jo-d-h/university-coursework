#pragma once

#include <cmath>

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float xValue, float yValue) : x(xValue), y(yValue) {}

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }

    Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    float lengthSquared() const { return x * x + y * y; }
    float length() const { return std::sqrt(lengthSquared()); }
};

// 두 벡터의 내부 계산
inline float dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

// 벡터를 단위 벡터로 변환
inline Vec2 normalize(const Vec2& value)
{
    const float len = value.length();
    if (len <= 0.0001f)
    {
        return Vec2(0.0f, 0.0f);
    }
    return Vec2(value.x / len, value.y / len);
}
