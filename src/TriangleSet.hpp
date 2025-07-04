#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>

#include "utils.hpp"
#include "Shape.hpp"


class TriangleSet : public Shape {
    
    private :

        std::vector<triangle> triangleArray;
        std::vector<FlatBVHNode> nodes;
        int rootIndex;
    
    public :

        TriangleSet(std::string filename);
        bool intersect(const ray& ray, HitInfo& globalHit);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        std::unique_ptr<BVHNode> buildBVH(std::vector<triangle>& triangles, int depth = 0);
        int flattenBVH(const std::unique_ptr<BVHNode>& node);
        ~TriangleSet() = default;

};


void printStats(const std::unique_ptr<BVHNode>& root);