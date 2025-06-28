#include "utils.hpp"


mat4 id() {
    return {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}


mat4 mTranslate(float3 a) {
    return {
        {1, 0, 0, a.x},
        {0, 1, 0, a.y},
        {0, 0, 1, a.z},
        {0, 0, 0, 1}
    };
}


mat4 mRotate(float3 axis, float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1 - c;
    float3 n = normalize(axis);
    return {
        {t * n.x * n.x + c, t * n.x * n.y - s * n.z, t * n.x * n.z + s * n.y, 0},
        {t * n.x * n.y + s * n.z, t * n.y * n.y + c, t * n.y * n.z - s * n.x, 0},
        {t * n.x * n.z - s * n.y, t * n.y * n.z + s * n.x, t * n.z * n.z + c, 0},
        {0, 0, 0, 1}
    };
}


mat4 mScale(float3 a) {
    return {
        {a.x, 0, 0, 0},
        {0, a.y, 0, 0},
        {0, 0, a.z, 0},
        {0, 0, 0, 1}
    };
}


float3 mul(mat4 a, float3 b) {
    return {
        dot(a.row0, {b.x, b.y, b.z, 1}),
        dot(a.row1, {b.x, b.y, b.z, 1}),
        dot(a.row2, {b.x, b.y, b.z, 1})
    };
}


float dot(float3 a, float3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


float3 cross(float3 a, float3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}


float3 mul(float a, float3 b) {
    return {a * b.x, a * b.y, a * b.z};
}


float dot(float4 a, float4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


float3 normalize(float3 a) {
    float length = std::sqrt(dot(a, a));
    return {a.x / length, a.y / length, a.z / length};
}


#include "Ray.hpp"


bool AABB::intersect(Ray *ray) {
    float3 dirfrac = ray->getInverseDirection();

    float t1 = (min.x - ray->getOrigin().x) * dirfrac.x;
    float t2 = (max.x - ray->getOrigin().x) * dirfrac.x;
    float t3 = (min.y - ray->getOrigin().y) * dirfrac.y;
    float t4 = (max.y - ray->getOrigin().y) * dirfrac.y;
    float t5 = (min.z - ray->getOrigin().z) * dirfrac.z;
    float t6 = (max.z - ray->getOrigin().z) * dirfrac.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) {
        return false;
    } else {
        return true;
    }
}


AABB AABB::fromTriangle(const triangle& tri) {
    float3 min = {
        std::min({tri.v0.x, tri.v1.x, tri.v2.x}),
        std::min({tri.v0.y, tri.v1.y, tri.v2.y}),
        std::min({tri.v0.z, tri.v1.z, tri.v2.z})
    };

    float3 max = {
        std::max({tri.v0.x, tri.v1.x, tri.v2.x}),
        std::max({tri.v0.y, tri.v1.y, tri.v2.y}),
        std::max({tri.v0.z, tri.v1.z, tri.v2.z})
    };

    return AABB{min, max};
}