#pragma once


#include <memory>
#include <string>
#include <vector>

#include "../utils.hpp"
#include "Shape.hpp"


class Mesh : public Shape {

    private:

        std::vector<Triangle> m_triangleArray;
        std::vector<FlatBVHNode> m_nodes;
        int m_rootIndex = -1;

    public:

        Mesh() = default;
        explicit Mesh(const std::string& filename);
        void construct(std::vector<Triangle>& triangles);
        bool intersect(const Ray& ray, HitInfo& globalHit) override;
        void translate(float3 translation) override;
        void rotate(float3 axis, float angle) override;
        void scale(float3 scaling) override;
        [[nodiscard]] std::unique_ptr<BVHNode> buildBVH(std::vector<Triangle>& triangles, int depth = 0);
        int flattenBVH(const std::unique_ptr<BVHNode>& node);
        ~Mesh() override = default;

};


void printStats(const std::unique_ptr<BVHNode>& root);
