#pragma once


#include "Shape.hpp"
#include "utils.hpp"
#include "Ray.hpp"


class TriangleSet : public Shape {
    
    private :

        std::vector<triangle> *triangles;
        std::pair<float3, float3> boundingBox;

    public :

        TriangleSet(std::string filename);

        std::vector<triangle> *getTriangles();
        void computeBoundingBox();
        void setTriangles(std::vector<triangle> *triangles);
        std::pair<float3, float3> intersect(Ray *ray);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~TriangleSet();

};