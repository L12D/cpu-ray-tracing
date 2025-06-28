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
        std::pair<float3, float3> intersect(Ray *ray);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        std::unique_ptr<BVHNode> buildBVH(std::vector<triangle>& triangles, int depth = 0);
        ~TriangleSet() = default;

};