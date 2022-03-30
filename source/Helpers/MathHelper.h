#pragma once

#ifndef PI
#define PI 3.14159265
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#include "types.h"

/**
 * Convert the angle given in radians to degrees.
 */
template<typename F>
F rad2deg(F angle) {
    return angle * 180.0 / M_PI;
}

class MathHelper {
public:
    static uint32_t		Abs(int32_t value) { return (value > 0 ? value : -value); }
    static int32_t		Max(int32_t value1, int32_t value2) { return (value1 > value2 ? value1 : value2); }
    static int32_t		Min(int32_t value1, int32_t value2) { return (value1 < value2 ? value1 : value2); }
    static int32_t		Sgn(int32_t value) { return (value == (int32_t)Abs(value) ? 1 : -1); }
    static OOFloat	    Frc(OOFloat value) { return value - (int)value; }
    static double CosineInterpolate(double y1, double y2, double mu);

    static uint32_t Random(uint32_t min, uint32_t max);
    static uint64_t mSeed;
};

// std::lerp implementation by (c) Edward Smith-Rowland
namespace std
{

    template<typename _Float>
    constexpr std::enable_if_t<std::is_floating_point_v<_Float>, _Float>
    lerp(_Float __a, _Float __b, _Float __t)
    {
        if (std::isnan(__a) || std::isnan(__b) || std::isnan(__t))
            return std::numeric_limits<_Float>::quiet_NaN();
        else if ((__a <= _Float{0} && __b >= _Float{0})
                 || (__a >= _Float{0} && __b <= _Float{0}))
            // ab <= 0 but product could overflow.
#ifndef FMA
            return __t * __b + (_Float{1} - __t) * __a;
#else
            return std::fma(__t, __b, (_Float{1} - __t) * __a);
#endif
        else if (__t == _Float{1})
            return __b;
        else
        { // monotonic near t == 1.
#ifndef FMA
            const auto __x = __a + __t * (__b - __a);
#else
            const auto __x = std::fma(__t, __b - __a, __a);
#endif
            return (__t > _Float{1}) == (__b > __a)
                   ? std::max(__b, __x)
                   : std::min(__b, __x);
        }
    }

} // namespace std


///// Vectors.h
//// =========
//// 2D/3D/4D vectors
////
////  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
//// CREATED: 2007-02-14
//// UPDATED: 2013-01-20
////
//// Copyright (C) 2007-2013 Song Ho Ahn

#include "ostream"
#define VECTOR_NORMALIZE_EPSILON 0.000001f

struct Vector2
{
    float x;
    float y;

    // ctors
    Vector2() : x(0), y(0) {};
    Vector2(float x, float y) : x(x), y(y) {};

    // utils functions
    void 		zero();
    void        set(float x, float y);
    float       length() const;                         //
    float       distance(const Vector2& vec) const;     // distance between two vectors
    Vector2&    normalize();                            //
    float       dot(const Vector2& vec) const;          // dot product
    bool        equal(const Vector2& vec, float e) const; // compare with epsilon

    // operators
    Vector2     operator-() const;                      // unary operator (negate)
    Vector2     operator+(const Vector2& rhs) const;    // add rhs
    Vector2     operator-(const Vector2& rhs) const;    // subtract rhs
    Vector2&    operator+=(const Vector2& rhs);         // add rhs and update this object
    Vector2&    operator-=(const Vector2& rhs);         // subtract rhs and update this object
    Vector2     operator*(const float scale) const;     // scale
    Vector2     operator*(const Vector2& rhs) const;    // multiply each element
    Vector2&    operator*=(const float scale);          // scale and update this object
    Vector2&    operator*=(const Vector2& rhs);         // multiply each element and update this object
    Vector2     operator/(const float scale) const;     // inverse scale
    Vector2&    operator/=(const float scale);          // scale and update this object
    bool        operator==(const Vector2& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Vector2& rhs) const;   // exact compare, no epsilon
    bool        operator<(const Vector2& rhs) const;    // comparison for sort
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Vector2 operator*(const float a, const Vector2 vec);
    friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
};

///////////////////////////////////////////////////////////////////////////////
// inline functions for Vector2
///////////////////////////////////////////////////////////////////////////////
inline Vector2 Vector2::operator-() const {
    return Vector2(-x, -y);
}

inline Vector2 Vector2::operator+(const Vector2& rhs) const {
    return Vector2(x+rhs.x, y+rhs.y);
}

inline Vector2 Vector2::operator-(const Vector2& rhs) const {
    return Vector2(x-rhs.x, y-rhs.y);
}

inline Vector2& Vector2::operator+=(const Vector2& rhs) {
    x += rhs.x; y += rhs.y; return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& rhs) {
    x -= rhs.x; y -= rhs.y; return *this;
}

inline Vector2 Vector2::operator*(const float a) const {
    return Vector2(x*a, y*a);
}

inline Vector2 Vector2::operator*(const Vector2& rhs) const {
    return Vector2(x*rhs.x, y*rhs.y);
}

inline Vector2& Vector2::operator*=(const float a) {
    x *= a; y *= a; return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& rhs) {
    x *= rhs.x; y *= rhs.y; return *this;
}

inline Vector2 Vector2::operator/(const float a) const {
    return Vector2(x/a, y/a);
}

inline Vector2& Vector2::operator/=(const float a) {
    x /= a; y /= a; return *this;
}

inline bool Vector2::operator==(const Vector2& rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}

inline bool Vector2::operator!=(const Vector2& rhs) const {
    return (x != rhs.x) || (y != rhs.y);
}

inline bool Vector2::operator<(const Vector2& rhs) const {
    if(x < rhs.x) return true;
    if(x > rhs.x) return false;
    if(y < rhs.y) return true;
    if(y > rhs.y) return false;
    return false;
}

inline float Vector2::operator[](int index) const {
    return (&x)[index];
}

inline float& Vector2::operator[](int index) {
    return (&x)[index];
}

inline void Vector2::zero() {
    this->x = 0.0f; this->y = 0.0f;
}

inline void Vector2::set(float x, float y) {
    this->x = x; this->y = y;
}

inline float Vector2::length() const {
    return std::sqrt(x*x + y*y);
}

inline float Vector2::distance(const Vector2& vec) const {
    return std::sqrt((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y));
}

inline Vector2& Vector2::normalize() {
    float xxyy = x*x + y*y;
    if(xxyy < VECTOR_NORMALIZE_EPSILON)
        return *this; // do nothing if it is ~zero vector

    ///float invLength = invSqrt(xxyy);
    float invLength = 1.0f / std::sqrt(xxyy);
    x *= invLength;
    y *= invLength;
    return *this;
}

inline float Vector2::dot(const Vector2& rhs) const {
    return (x*rhs.x + y*rhs.y);
}

inline bool Vector2::equal(const Vector2& rhs, float epsilon) const {
    return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

inline Vector2 operator*(const float a, const Vector2 vec) {
    return Vector2(a*vec.x, a*vec.y);
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
// END OF VECTOR2 /////////////////////////////////////////////////////////////

