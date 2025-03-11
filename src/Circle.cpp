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


void Circle::intersect(Ray *ray, cv::Vec3b color) {
    float3 dir = ray->getDirection();
    float3 oc = dir - this->center;
    float b = 2.0f * dot(oc, dir);
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4.0f * c;  // Since a = 1, 4a = 4

    if (discriminant < 0.0f) {
        ray->setLength(10000.0); // No intersection
        return;
    }

    float sqrtDiscriminant = sqrt(discriminant);

    float t0 = (-b - sqrtDiscriminant) * 0.5f;  // Dividing by 2 directly
    float t1 = (-b + sqrtDiscriminant) * 0.5f;

    // Return the closest valid intersection point
    if (t0 > 0.0f) {
        ray->setLength(t0);
        ray->setColor(color);
        return;
    } else if (t1 > 0.0f) {
        ray->setLength(t1);
        ray->setColor(color);
        return;
    }

    ray->setLength(10000.0);
}
