#include "Ray.hpp"


Ray::Ray(float3 origin, float3 direction) {
    this->origin = origin;
    this->direction = direction;
    this->length = 0;
    this->color = cv::Vec3b(0, 0, 0);
}


float3 Ray::getOrigin() {
    return this->origin;
}


void Ray::setOrigin(float3 origin) {
    this->origin = origin;
}


float3 Ray::getDirection() {
    return this->direction;
}


void Ray::setDirection(float3 direction) {
    this->direction = direction;
}


float Ray::getLength() {
    return this->length;
}


void Ray::setLength(float length) {
    this->length = length;
}


cv::Vec3b Ray::getColor() {
    return this->color;
}


void Ray::setColor(cv::Vec3b color) {
    this->color = color;
}