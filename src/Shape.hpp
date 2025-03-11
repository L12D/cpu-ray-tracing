#pragma once


#include "Ray.hpp"


class Shape {
    
    public :

        virtual void intersect(Ray *ray) = 0;
        virtual ~Shape() = default;

};
