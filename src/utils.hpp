#pragma once


#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <random>


#define CLAMP01(v) ((v) < 0.0f ? 0.0f : ((v) > 1.0f ? 1.0f : (v)))


struct float3 {
    float x, y, z;

    float3() : x(0), y(0), z(0) {}
    float3(float x, float y, float z) : x(x), y(y), z(z) {}

    float3 operator+(const float3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    float3 operator*(const float3& other) const {
        return {x * other.x, y * other.y, z * other.z};
    }

    float3 operator-(const float3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    float3 operator-() const {
        return {-x, -y, -z};
    }

    friend std::ostream& operator<<(std::ostream& os, const float3& f) {
        os << "{" << f.x << ", " << f.y << ", " << f.z << "}";
        return os;
    }
    
    float3 clamp() const {
        return {CLAMP01(x), CLAMP01(y), CLAMP01(z)};
    }
};

float dot(float3 a, float3 b);
float3 cross(float3 a, float3 b);
float3 mul(float a, float3 b);
float3 normalize(float3 a);


struct float4 {
    float x, y, z, w;

    float4() : x(0), y(0), z(0), w(0) {}
    float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    float4(float3 v) : x(v.x), y(v.y), z(v.z), w(1) {}

    float4 operator+(const float4& other) {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    float4 operator*(const float4& other) {
        return {x * other.x, y * other.y, z * other.z, w * other.w};
    }

    friend std::ostream& operator<<(std::ostream& os, const float4& f) {
        os << "{" << f.x << ", " << f.y << ", " << f.z << ", " << f.w << "}";
        return os;
    }
};


float dot(float4 a, float4 b);


struct mat4 {
    float4 row0, row1, row2, row3;

    mat4 operator*(const mat4& other) {
        return {
            {
                dot(row0, {other.row0.x, other.row1.x, other.row2.x, other.row3.x}),
                dot(row0, {other.row0.y, other.row1.y, other.row2.y, other.row3.y}),
                dot(row0, {other.row0.z, other.row1.z, other.row2.z, other.row3.z}),
                dot(row0, {other.row0.w, other.row1.w, other.row2.w, other.row3.w})
            },
            {
                dot(row1, {other.row0.x, other.row1.x, other.row2.x, other.row3.x}),
                dot(row1, {other.row0.y, other.row1.y, other.row2.y, other.row3.y}),
                dot(row1, {other.row0.z, other.row1.z, other.row2.z, other.row3.z}),
                dot(row1, {other.row0.w, other.row1.w, other.row2.w, other.row3.w})
            },
            {
                dot(row2, {other.row0.x, other.row1.x, other.row2.x, other.row3.x}),
                dot(row2, {other.row0.y, other.row1.y, other.row2.y, other.row3.y}),
                dot(row2, {other.row0.z, other.row1.z, other.row2.z, other.row3.z}),
                dot(row2, {other.row0.w, other.row1.w, other.row2.w, other.row3.w})
            },
            {
                dot(row3, {other.row0.x, other.row1.x, other.row2.x, other.row3.x}),
                dot(row3, {other.row0.y, other.row1.y, other.row2.y, other.row3.y}),
                dot(row3, {other.row0.z, other.row1.z, other.row2.z, other.row3.z}),
                dot(row3, {other.row0.w, other.row1.w, other.row2.w, other.row3.w})
            }
        };
    }
};


struct ray {
    float3 origin;
    float3 direction;
    float3 inverseDirection;
    
    ray() : origin({0, 0, 0}), direction({0, 0, 1}), inverseDirection({1e6f, 1e6f, 1e6f}) {}

    ray(float3 origin, float3 direction) : origin(origin), direction(direction) {
        inverseDirection = {1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z};
        if (inverseDirection.x == 0) inverseDirection.x = 1e6f; // Avoid division by zero
        if (inverseDirection.y == 0) inverseDirection.y = 1e6f; // Avoid division by zero
        if (inverseDirection.z == 0) inverseDirection.z = 1e6f; // Avoid division by zero
    }
};


std::vector<ray> generateRays(float3 origin, float3 normal, float3 direction, int n);
ray getMirrorRay(float3 intersectionPoint, float3 normal, float3 incident);


struct triangle {
    float3 v0, v1, v2;

    triangle(float3 v0, float3 v1, float3 v2) : v0(v0), v1(v1), v2(v2) {}

    float3 centroid() const {
        return {(v0.x + v1.x + v2.x) / 3, (v0.y + v1.y + v2.y) / 3, (v0.z + v1.z + v2.z) / 3};
    }
};


mat4 id();
mat4 mTranslate(float3 a);
mat4 mRotate(float3 axis, float angle); // angle in radians
mat4 mScale(float3 a);
float3 mul(mat4 a, float3 b);


struct AABB {
    float3 min;
    float3 max;

    float intersect(const ray& ray) const;

    void expand(const AABB& other) {
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

    static AABB fromTriangle(const triangle& p);
};


struct BVHNode {
    AABB boundingBox;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::unique_ptr<std::vector<triangle>> triangles;
    bool isLeaf = false;

    BVHNode() = default;
};


struct FlatBVHNode {
    AABB boundingBox;
    uint32_t leftChildIndex;
    uint32_t rightChildIndex;
    uint32_t triangleOffset;
    uint32_t triangleCount;
    bool isLeaf;
};


struct HitInfo {
    float3 position;
    float3 normal;
    float distance = std::numeric_limits<float>::max();
};