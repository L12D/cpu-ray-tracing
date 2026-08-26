#pragma once


#include "../utils.hpp"


class Shape {

    public:

        virtual ~Shape() = default;
        Shape() = default;
        Shape(const Shape&) = delete;
        Shape& operator=(const Shape&) = delete;

        virtual bool intersect(const Ray& ray, HitInfo& hit) = 0;
        virtual void translate(float3 translation) = 0;
        virtual void rotate(float3 axis, float angle) = 0;
        virtual void scale(float3 scaling) = 0;

};
