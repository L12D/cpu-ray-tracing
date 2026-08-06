#pragma once


#include "Shape.hpp"
#include "TriangleSet.hpp"


class Mesh: public Shape {

    private:

        TriangleSet *set;
        TriangleSet *partialSet;
    
    public:

        Mesh(std::string name);
        bool intersect(const ray& ray, HitInfo& globalHit);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~Mesh();

};