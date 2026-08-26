#pragma once


#include "Shape.hpp"
#include "../utils.hpp"


class Quad : public Shape {

    private:

        Triangle m_tri1;
        Triangle m_tri2;

    public:

        Quad();
        Quad(float3 base, float3 diagonal, float3 v1, float3 v2);
        bool intersect(const Ray& ray, HitInfo& globalHit) override;
        void translate(float3 translation) override;
        void rotate(float3 /*axis*/, float /*angle*/) override;
        void scale(float3 /*scaling*/) override;
        ~Quad() override = default;

};
