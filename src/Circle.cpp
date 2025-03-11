#include "Circle.hpp"


Circle::Circle(float3 center, float radius) {
    this->center = center;
    this->radius = radius;
}


float3 Circle::getCenter() {
    return this->center;
}


void Circle::setCenter(float3 center) {
    this->center = center;
}


float Circle::getRadius() {
    return this->radius;
}


void Circle::setRadius(float radius) {
    this->radius = radius;
}


bool Circle::intersect(float3 ray) {
    float3 oc = ray - this->center;
    float a = dot(ray, ray);
    float b = 2 * dot(oc, ray);
    float c = dot(oc, oc) - this->radius * this->radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant >= 0;
}
