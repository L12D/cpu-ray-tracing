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


// bool intersect_triangle(float3 ray, float3 v0, float3 v1, float3 v2) {
//     float3 edge1 = v1 - v0;
//     float3 edge2 = v2 - v0;
//     float3 h = cross(ray, edge2);
//     float a = dot(edge1, h);
//     if (a > -0.00001 && a < 0.00001) {
//         return false; // This ray is parallel to this triangle.
//     }
//     float f = 1.0 / a;
//     float3 s = -v0;
//     float u = f * dot(s, h);
//     if (u < 0.0 || u > 1.0) {
//         return false;
//     }
//     float3 q = cross(s, edge1);
//     float v = f * dot(ray, q);
//     if (v < 0.0 || u + v > 1.0) {
//         return false;
//     }
//     float t = f * dot(edge2, q);
//     if (t > 0.00001) { // ray intersection
//         return true;
//     } else {
//         return false;
//     }
// }
