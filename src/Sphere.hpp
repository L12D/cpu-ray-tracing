#pragma once


#include "Shape.hpp"
#include "utils.hpp"
#include "Ray.hpp"


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
        std::pair<float3, float3> intersect(Ray *ray);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~Sphere() = default;

};