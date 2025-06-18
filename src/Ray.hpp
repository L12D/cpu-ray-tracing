#pragma once


#include <opencv2/opencv.hpp>
#include <random>

#include "utils.hpp"


class Ray {
    
    private :

        float3 origin;
        float3 direction;
        float length;
        float3 color; // BGR
        bool hit;
    
    public :

        Ray(float3 origin, float3 direction);
        float3 getOrigin();
        void setOrigin(float3 origin);
        float3 getDirection();
        void setDirection(float3 direction);
        float getLength();
        void setLength(float length);
        float3 getColor();
        void setColor(float3 color);
        bool getHit();
        void setHit(bool hit);
        ~Ray() = default;

};


std::vector<Ray *> generateRays(float3 origin, float3 normal, float3 direction, int n);
float3 randomDirection(std::mt19937 gen, std::uniform_real_distribution<float> dis);
