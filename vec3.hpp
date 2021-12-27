#include <cmath>
#include <ctype.h>
#include <concepts>
#include <assert.h>
#include <span>
#include <ostream>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

/// @brief Uncomplete, humble vec3 library
template <Arithmetic T>
struct vec3
{
    T x = 0, y = 0, z = 0;

    template <Arithmetic U>
    void operator+=(const vec3<U> &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    template <Arithmetic U>
    void operator+=(const U &v)
    {
        x += v;
        y += v;
        z += v;
    }

    template <Arithmetic U>
    void operator-=(const vec3<U> &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    template <Arithmetic U>
    void operator-=(const U &v)
    {
        x -= v;
        y -= v;
        z -= v;
    }

    template <Arithmetic U>
    void operator/=(const U &v)
    {
        x /= v;
        y /= v;
        z /= v;
    }

    T &operator[](const size_t &idx)
    {
        switch (idx)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            assert(false);
        }
    }

    const T &operator[](const size_t &idx) const
    {
        switch (idx)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            assert(false);
        }
    }

    template <class = typename std::enable_if<std::is_signed_v<T>>>
    vec3<T> operator-() { return {-x, -y, -z}; }

    vec3<T> abs()
    {
        return vec3<T>{std::abs(x), std::abs(y), std::abs(z)};
    }
};

template <Arithmetic T, Arithmetic U>
bool operator==(const vec3<T> &a, const vec3<U> &b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <Arithmetic T, Arithmetic U>
bool operator!=(const vec3<T> &a, const vec3<U> &b)
{
    return a.x != b.x && a.y != b.y && a.z != b.z;
}

template <Arithmetic T, Arithmetic U>
vec3<T> operator+(const vec3<T> &a, const vec3<U> &b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

template <Arithmetic T, Arithmetic U>
vec3<T> operator-(const vec3<T> &a, const vec3<U> &b)
{
    return vec3<T>{a.x - b.x, a.y - b.y, a.z - b.z};
}

template <Arithmetic T, Arithmetic U>
vec3<T> operator/(const vec3<T> &a, const vec3<U> &b)
{
    return {a.x / b.x, a.y / b.y, a.z / b.z};
}

template <Arithmetic T, Arithmetic U>
vec3<T> operator*(const vec3<T> &a, const vec3<U> &b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

template <Arithmetic T>
std::ostream &operator<<(std::ostream &os, const vec3<T> &vec)
{
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

template <Arithmetic T>
vec3<T> floor(const vec3<T> &vec)
{
    return vec3<T>{std::floor(vec.x), std::floor(vec.y), std::floor(vec.z)};
}

// template <Arithmetic T>
// vec3<T> abs(const vec3<T> &vec)
// {
//     return vec3<T>{std::abs(vec.x), std::abs(vec.y), std::abs(vec.z)};
// }

template <Arithmetic T>
T euclidean_distance(const vec3<T> &a, const vec3<T> &b)
{
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2));
}

template <Arithmetic T>
T manhattan_distance(const vec3<T> &a, const vec3<T> &b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}