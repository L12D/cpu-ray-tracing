#pragma once


#include "../utils.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"


class Cube : public Shape {

    private:

        Mesh m_mesh;

    public:

        Cube();
        Cube(float3 base, float3 v1, float3 v2, float3 v3);
        bool intersect(const Ray& ray, HitInfo& globalHit) override;
        void translate(float3 translation) override;
        void rotate(float3 axis, float angle) override;
        void scale(float3 scaling) override;
        ~Cube() override = default;

};
