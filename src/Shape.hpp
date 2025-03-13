#pragma once


#include "Ray.hpp"


class Shape {
    
    public :

        virtual std::pair<float3, float3> intersect(Ray *ray, int depth) = 0;
        virtual ~Shape() = default;

};
