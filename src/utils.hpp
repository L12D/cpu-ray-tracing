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
