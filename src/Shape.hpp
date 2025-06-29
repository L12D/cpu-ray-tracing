#pragma once


#include "Ray.hpp"


class Shape {
    
    public :

        virtual bool intersect(Ray *ray, HitInfo& hit) = 0;
        virtual void translate(float3 translation) = 0;
        virtual void rotate(float3 axis, float angle) = 0;
        virtual void scale(float3 scaling) = 0;
        virtual ~Shape() = default;

};
