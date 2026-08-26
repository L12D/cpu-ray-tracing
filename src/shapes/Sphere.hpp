#pragma once


#include "Shape.hpp"
#include "../utils.hpp"


class Sphere : public Shape {

    private:

        float3 m_center;
        float m_radius;

    public:

        Sphere(float3 center, float radius);
        [[nodiscard]] float3 getCenter() const noexcept;
        void setCenter(float3 center) noexcept;
        [[nodiscard]] float getRadius() const noexcept;
        void setRadius(float radius) noexcept;
        bool intersect(const Ray& ray, HitInfo& hit) override;
        void translate(float3 translation) override;
        void rotate(float3 /*axis*/, float /*angle*/) override;
        void scale(float3 scaling) override;
        ~Sphere() override = default;

};
