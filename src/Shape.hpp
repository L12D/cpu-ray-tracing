#pragma once


#include "Ray.hpp"


class Shape {
    
    public :

        virtual void intersect(Ray *ray, cv::Vec3b color) = 0;
        virtual ~Shape() = default;

};
