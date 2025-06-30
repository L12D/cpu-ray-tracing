#pragma once


#include <vector>

#include "Shape.hpp"
#include "utils.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Application.hpp"


class Object {

    private :

        Shape *shape;
        float3 color; // BGR
        bool isLight;
        bool isMirror;
        std::vector<Object *> children;
    
    public :

        Object();
        Object(Shape *shape, float3 color);
        Shape *getShape();
        void setShape(Shape *shape);
        float3 getColor();
        void setColor(float3 color);
        void setLight();
        void setMirror();
        void intersect(Ray *ray, HitInfo &hit, int depth, int maxDepth);
        float3 getRayColor(float3 intersectionPoint, float3 normal, float3 incident, int depth, int maxDepth);
        void translate(float3 translation);
        void rotate(float3 axis, float angle);
        void scale(float3 scaling);
        ~Object();

};
