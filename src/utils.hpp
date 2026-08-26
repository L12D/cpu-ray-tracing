#pragma once


#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>


inline constexpr double PI = 3.14159265358979323846; // same value as M_PI

inline constexpr bool PRODUCTION = false;
inline constexpr int RESOLUTION = 540;
inline constexpr int SCENE = 5;
inline constexpr int N_RAYS = 10;
inline constexpr float INV_N_RAYS = 1.0f / static_cast<float>(N_RAYS);

inline constexpr int MAX_RENDER_DEPTH = 3;
inline constexpr int MAX_ACTUAL_DEPTH = 10;
inline constexpr float MIRROR_REFLECTIVENESS = 0.85f;

inline constexpr float FOV = static_cast<float>(PI / 3.1f);
inline constexpr float FLOAT_MAX = std::numeric_limits<float>::max();


[[nodiscard]] inline constexpr float clamp01(float v) noexcept {
    return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
}


struct float3 {
    float x, y, z;

    constexpr float3() noexcept : x(0), y(0), z(0) {}
    constexpr float3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

    [[nodiscard]] float3 operator+(const float3& other) const noexcept {
        return {x + other.x, y + other.y, z + other.z};
    }

    [[nodiscard]] float3 operator*(const float3& other) const noexcept {
        return {x * other.x, y * other.y, z * other.z};
    }

    [[nodiscard]] float3 operator-(const float3& other) const noexcept {
        return {x - other.x, y - other.y, z - other.z};
    }

    [[nodiscard]] float3 operator-() const noexcept {
        return {-x, -y, -z};
    }

    [[nodiscard]] float3 operator*(float scalar) const noexcept {
        return {x * scalar, y * scalar, z * scalar};
    }

    friend float3 operator*(float scalar, const float3& v) noexcept {
        return v * scalar;
    }

    friend std::ostream& operator<<(std::ostream& os, const float3& f) {
        os << "{" << f.x << ", " << f.y << ", " << f.z << "}";
        return os;
    }

    [[nodiscard]] float3 clamp() const noexcept {
        return {clamp01(x), clamp01(y), clamp01(z)};
    }
};


inline constexpr float3 BC_COLOR_1 {0.3f, 0.3f, 0.3f}; // background when no hit
inline constexpr float3 BC_COLOR_2 {0.3f, 0.3f, 0.3f}; // ambient/default color


[[nodiscard]] inline float dot(const float3& a, const float3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

[[nodiscard]] inline float3 cross(const float3& a, const float3& b) noexcept {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

[[nodiscard]] inline float3 normalize(const float3& a) noexcept {
    const float length = std::sqrt(dot(a, a));
    return {a.x / length, a.y / length, a.z / length};
}


struct float4 {
    float x, y, z, w;

    constexpr float4() noexcept : x(0), y(0), z(0), w(0) {}
    constexpr float4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
    constexpr explicit float4(const float3& v) noexcept : x(v.x), y(v.y), z(v.z), w(1) {}

    [[nodiscard]] float4 operator+(const float4& other) const noexcept {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    [[nodiscard]] float4 operator*(const float4& other) const noexcept {
        return {x * other.x, y * other.y, z * other.z, w * other.w};
    }

    friend std::ostream& operator<<(std::ostream& os, const float4& f) {
        os << "{" << f.x << ", " << f.y << ", " << f.z << ", " << f.w << "}";
        return os;
    }
};


[[nodiscard]] inline float dot(const float4& a, const float4& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


struct mat4 {
    float4 row0, row1, row2, row3;

    [[nodiscard]] mat4 operator*(const mat4& other) const noexcept {
        const float4 colX {other.row0.x, other.row1.x, other.row2.x, other.row3.x};
        const float4 colY {other.row0.y, other.row1.y, other.row2.y, other.row3.y};
        const float4 colZ {other.row0.z, other.row1.z, other.row2.z, other.row3.z};
        const float4 colW {other.row0.w, other.row1.w, other.row2.w, other.row3.w};

        return {
            {dot(row0, colX), dot(row0, colY), dot(row0, colZ), dot(row0, colW)},
            {dot(row1, colX), dot(row1, colY), dot(row1, colZ), dot(row1, colW)},
            {dot(row2, colX), dot(row2, colY), dot(row2, colZ), dot(row2, colW)},
            {dot(row3, colX), dot(row3, colY), dot(row3, colZ), dot(row3, colW)}
        };
    }
};


struct Ray {
    float3 origin;
    float3 direction;
    float3 inverseDirection;

    Ray() noexcept : origin({0, 0, 0}), direction({0, 0, 1}), inverseDirection({1e6f, 1e6f, 1e6f}) {}

    Ray(float3 origin, float3 direction) : origin(origin), direction(direction) {
        inverseDirection = {1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z};
        if (inverseDirection.x == 0) inverseDirection.x = 1e6f; // Avoid division by zero
        if (inverseDirection.y == 0) inverseDirection.y = 1e6f; // Avoid division by zero
        if (inverseDirection.z == 0) inverseDirection.z = 1e6f; // Avoid division by zero
    }
};


struct Triangle {
    float3 v0, v1, v2, edge1, edge2, normal;

    Triangle(float3 v0, float3 v1, float3 v2)
        : v0(v0), v1(v1), v2(v2), edge1(v1 - v0), edge2(v2 - v0),
          normal(normalize(cross(edge1, edge2))) {}

    [[nodiscard]] float3 centroid() const noexcept {
        return {(v0.x + v1.x + v2.x) / 3, (v0.y + v1.y + v2.y) / 3, (v0.z + v1.z + v2.z) / 3};
    }
};


struct AABB {
    float3 min;
    float3 max;

    [[nodiscard]] float intersect(const Ray& ray) const noexcept;

    void expand(const AABB& other) noexcept {
        min = {
            std::min(min.x, other.min.x),
            std::min(min.y, other.min.y),
            std::min(min.z, other.min.z)
        };
        max = {
            std::max(max.x, other.max.x),
            std::max(max.y, other.max.y),
            std::max(max.z, other.max.z)
        };
    }

    [[nodiscard]] static AABB fromTriangle(const Triangle& tri) noexcept;

    [[nodiscard]] float surfaceArea() const noexcept {
        const float3 extent = max - min;
        return 2.0f * (extent.x * extent.y + extent.y * extent.z + extent.z * extent.x);
    }
};


struct BVHNode {
    AABB boundingBox;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<Triangle> triangles;
    bool isLeaf = false;
};


struct FlatBVHNode {
    AABB boundingBox;
    uint32_t leftChildIndex;
    uint32_t rightChildIndex;
    uint32_t triangleOffset;
    uint32_t triangleCount;
};

inline constexpr uint32_t INVALID_NODE_INDEX = std::numeric_limits<uint32_t>::max();


struct HitInfo {
    int renderDepth = 0; // increased when the ray that generates a number of rays (not increased when it's a mirror)
    int actualDepth = 0; // increased every hit
    float3 position;
    float3 normal;
    float distance = FLOAT_MAX;
};


Ray generateRay(float3 origin, float3 normal, float3 /*direction*/);
Ray getMirrorRay(float3 intersectionPoint, float3 normal, float3 incident);
std::vector<Ray> generateRays(float3 origin, float3 normal, float3 /*direction*/);


mat4 identity();
mat4 mTranslate(float3 a);
mat4 mRotate(float3 axis, float angle); // angle in radians
mat4 mScale(float3 a);


bool intersectOrientedTriangle(const Triangle& tri, const Ray& ray, HitInfo& hit);
bool intersectTriangle(const Triangle& tri, const Ray& ray, HitInfo& hit);
