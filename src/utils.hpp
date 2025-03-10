#pragma once


#include <iostream>
#include <cmath>


struct float3 {
    float x, y, z;

    float3 operator+(const float3& other) {
        return {x + other.x, y + other.y, z + other.z};
    }

    float3 operator*(const float3& other) {
        return {x * other.x, y * other.y, z * other.z};
    }

    friend std::ostream& operator<<(std::ostream& os, const float3& f) {
        os << "{" << f.x << ", " << f.y << ", " << f.z << "}";
        return os;
    }
};

float dot(float3 a, float3 b);
float3 normalize(float3 a);


struct float4 {
    float x, y, z, w;

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


mat4 id();
mat4 translate(float3 a);
mat4 rotate(float3 axis, float angle); // angle in radians
mat4 scale(float3 a);
float3 mul(mat4 a, float3 b);
