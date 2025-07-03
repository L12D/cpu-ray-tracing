#pragma once


#include "Shape.hpp"
#include "utils.hpp"


class Sphere : public Shape {
    
    private :

        float3 center;
        float radius;
    
    public :

        Sphere(float3 center, float radius);
        float3 getCenter();
        void setCenter(float3 center);
        float getRadius();
        void setRadius(float radius);
        bool intersect(const ray& ray, HitInfo& hit);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~Sphere() = default;

};