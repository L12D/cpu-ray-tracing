#pragma once


#include <vector>

#include "Shape.hpp"
#include "utils.hpp"
#include "Ray.hpp"
#include "Scene.hpp"


class Object {

    private :

        Shape *shape;
        float3 color; // BGR
        bool isLight;
        std::pair<float3, float3> bounding_box;
        std::vector<Object *> children;
    
    public :

        Object();
        Object(Shape *shape, float3 color, bool isLight);
        Shape *getShape();
        void setShape(Shape *shape);
        float3 getColor();
        void setColor(float3 color);
        bool getIsLight();
        void setIsLight(bool isLight);
        void intersect(Ray *ray, int depth, int maxDepth);
        ~Object();

};
