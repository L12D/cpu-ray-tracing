#pragma once


#include <opencv2/opencv.hpp>

#include "utils.hpp"


class Ray {
    
    private :

        float3 origin;
        float3 direction;
        float length;
        cv::Vec3b color;
    
    public :

        Ray(float3 origin, float3 direction);
        float3 getOrigin();
        void setOrigin(float3 origin);
        float3 getDirection();
        void setDirection(float3 direction);
        float getLength();
        void setLength(float length);
        cv::Vec3b getColor();
        void setColor(cv::Vec3b color);
        ~Ray() = default;

};
