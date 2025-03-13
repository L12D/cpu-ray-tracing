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


std::pair<float3, float3> Circle::intersect(Ray *ray, int depth) {
    // std::cout << "Intersecting circle at depth " << depth << std::endl;
    float3 dir = ray->getDirection();
    float3 oc = dir - this->center;
    float b = 2.0f * dot(oc, dir);
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4.0f * c;  // Since a = 1, 4a = 4

    if (discriminant < 0.0f) {
        ray->setHit(false);
        return {{0, 0, 0}, {0, 0, 0}};
    }

    float sqrtDiscriminant = sqrt(discriminant);

    float t0 = (-b - sqrtDiscriminant) * 0.5f;  // Dividing by 2 directly
    float t1 = (-b + sqrtDiscriminant) * 0.5f;

    // std::cout << "Ray origin: " << ray->getOrigin() << std::endl;
    // std::cout << "Ray direction: " << ray->getDirection() << std::endl;

    // Return the closest valid intersection point
    if (t0 > 0.0) {
        // std::cout << "Intersected ###################################################" << std::endl;
        ray->setHit(true);
        ray->setLength(t0);
        float3 intersectionPoint = ray->getOrigin() + mul(t0, ray->getDirection());
        return {intersectionPoint, normalize(intersectionPoint - this->center)};
    } else if (t1 > 0.0) {
        // std::cout << "Intersected ###################################################" << std::endl;
        ray->setHit(true);
        ray->setLength(t1);
        float3 intersectionPoint = ray->getOrigin() + mul(t1, ray->getDirection());
        return {intersectionPoint, normalize(intersectionPoint - this->center)};
    }

    ray->setHit(false);
    return {{0, 0, 0}, {0, 0, 0}};
}
