#include "Sphere.hpp"


Sphere::Sphere(float3 center, float radius) {
    this->center = center;
    this->radius = radius;
}


float3 Sphere::getCenter() {
    return this->center;
}


void Sphere::setCenter(float3 center) {
    this->center = center;
}


float Sphere::getRadius() {
    return this->radius;
}


void Sphere::setRadius(float radius) {
    this->radius = radius;
}


std::pair<float3, float3> Sphere::intersect(Ray *ray) {
    float3 dir = ray->getDirection();
    float3 oc = ray->getOrigin() - this->center;
    float b = 2.0f * dot(oc, dir);
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4.0f * c;  // Since a = 1

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
