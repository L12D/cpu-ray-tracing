#include "utils.hpp"


mat4 id() {
    return {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}


mat4 translate(float3 a) {
    return {
        {1, 0, 0, a.x},
        {0, 1, 0, a.y},
        {0, 0, 1, a.z},
        {0, 0, 0, 1}
    };
}


mat4 rotate(float3 axis, float angle) {
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


mat4 scale(float3 a) {
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


float dot(float4 a, float4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


float3 normalize(float3 a) {
    float length = std::sqrt(dot(a, a));
    return {a.x / length, a.y / length, a.z / length};
}
