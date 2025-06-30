#pragma once


#include <opencv2/opencv.hpp>
#include <random>

#include "utils.hpp"


class Ray {
    
    private :

        float3 origin;
        float3 direction;
        float3 inverseDirection;
        float3 color; // BGR
    
    public :

        Ray(float3 origin, float3 direction);
        float3 getOrigin();
        void setOrigin(float3 origin);
        float3 getDirection();
        float3 getInverseDirection();
        void setDirection(float3 direction);
        float3 getColor();
        void setColor(float3 color);
        ~Ray() = default;

};


std::vector<Ray *> generateRays(float3 origin, float3 normal, float3 direction, int n);
Ray *getMirrorRay(float3 intersectionPoint, float3 normal, float3 incident);
