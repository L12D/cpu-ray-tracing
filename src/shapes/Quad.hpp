#pragma once


#include "Shape.hpp"
#include "../utils.hpp"


class Quad: public Shape {

    private:
        triangle tri1;
        triangle tri2;

    public:
        Quad();
        Quad(float3 base, float3 diagonal, float3 v1, float3 v2);
        bool intersect(const ray& ray, HitInfo& globalHit);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~Quad() = default;
        
};