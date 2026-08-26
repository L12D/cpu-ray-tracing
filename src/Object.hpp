#pragma once


#include <vector>

#include "shapes/Shape.hpp"
#include "utils.hpp"
#include "Scene.hpp"
#include "Application.hpp"


class Object {

    private:

        std::unique_ptr<Shape> m_shape;
        float3 m_color; // BGR
        bool m_light = false;
        bool m_mirror = false;
        bool m_invisible = false;

    public:

        Object() = default;
        explicit Object(std::unique_ptr<Shape> shape, float3 color = {0, 0, 0});
        [[nodiscard]] Shape* getShape() const noexcept;
        void setShape(std::unique_ptr<Shape> shape) noexcept;
        [[nodiscard]] float3 getColor() const noexcept;
        void setColor(float3 color) noexcept;
        void setLight(bool light) noexcept;
        [[nodiscard]] bool isLight() const noexcept;
        void setMirror(bool mirror) noexcept;
        [[nodiscard]] bool isMirror() const noexcept;
        void setInvisible(bool invisible) noexcept;
        [[nodiscard]] bool isInvisible() const noexcept;
        void intersect(const Ray& ray, HitInfo& hit);
        [[nodiscard]] float3 getRayColor(float3 intersectionPoint, float3 normal, float3 incident, int renderDepth, int actualDepth);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);

};
