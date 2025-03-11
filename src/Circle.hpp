#pragma once


#include "Shape.hpp"
#include "utils.hpp"


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
        bool intersect(float3 ray);
        ~Circle() = default;

};