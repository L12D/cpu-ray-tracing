#pragma once


#include "Shape.hpp"
#include "utils.hpp"
#include "Ray.hpp"


class Circle : public Shape {
    
    private :

        float3 center;
        float radius;
    
    public :

        Circle(float3 center, float radius);
        float3 getCenter();
        void setCenter(float3 center);
        float getRadius();
        void setRadius(float radius);
        std::pair<float3, float3> intersect(Ray *ray, int depth);
        ~Circle() = default;

};