#pragma once


#include <vector>

#include "Shape.hpp"
#include "utils.hpp"


class Object {

    private :

        Shape *shape;
        std::pair<float3, float3> bounding_box;
        std::vector<Object *> children;
    
    public :

        Object();
        Object(Shape *shape);
        Shape *getShape();
        void setShape(Shape *shape);
        std::pair<float3, float3> computeBoundingBox();
        std::pair<float3, float3> getBoundingBox();
        ~Object();

};
