#pragma once


#include "Shape.hpp"
#include "utils.hpp"
#include "Ray.hpp"


class TriangleSet : public Shape {
    
    private :

        std::vector<triangle> triangles;

    public :

        TriangleSet(std::string filename);
        std::vector<triangle> getTriangles();
        void setTriangles(std::vector<triangle> triangles);
        std::pair<float3, float3> intersect(Ray *ray);
        ~TriangleSet() = default;

};