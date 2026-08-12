#pragma once


#include "../utils.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"


class Cube: public Shape {
    
    private:
        Mesh *mesh;

    public:
        Cube();
        Cube(float3 base, float3 v1, float3 v2, float3 v3);
        bool intersect(const ray& ray, HitInfo& globalHit);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~Cube();

};


void printStats(const std::unique_ptr<BVHNode>& root);