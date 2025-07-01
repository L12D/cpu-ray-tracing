#pragma once


#include "utils.hpp"
#include "Shape.hpp"
// #include "Ray.hpp"


class TriangleSet : public Shape {
    
    private :

        // std::unique_ptr<std::vector<triangle>> triangles;
        std::unique_ptr<BVHNode> root;
        // AABB boundingBox;

    public :

        TriangleSet(std::string filename);
        // void computeBoundingBox();
        bool intersect(Ray *ray, HitInfo& globalHit);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        void printStats();
        std::unique_ptr<BVHNode> buildBVH(std::vector<triangle>& triangles, int depth = 0);
        ~TriangleSet() = default;

};