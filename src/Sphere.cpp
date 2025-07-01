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


bool Sphere::intersect(Ray *ray, HitInfo& hit) {
    float3 dir = ray->getDirection();
    float3 oc = ray->getOrigin() - this->center;
    float b = 2.0f * dot(oc, dir);
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4.0f * c;  // Since a = 1

    if (discriminant < 0.0f) {
        return false;
    }

    float sqrtDiscriminant = sqrt(discriminant);

    float t0 = (-b - sqrtDiscriminant) * 0.5f;  // Dividing by 2 directly
    float t1 = (-b + sqrtDiscriminant) * 0.5f;

    // Return the closest valid intersection point
    if (t0 > 0.0) {
        hit.distance = t0;
        hit.position = ray->getOrigin() + mul(t0, ray->getDirection());
        hit.normal = normalize(hit.position - this->center);
        return true;
    } else if (t1 > 0.0) {
        hit.distance = t1;
        hit.position = ray->getOrigin() + mul(t1, ray->getDirection());
        hit.normal = normalize(hit.position - this->center);
        return true;
    }

    return false;
}


void Sphere::translate(float3 translation) {
    this->center = this->center + translation;
}


void Sphere::rotate(float3 axis, float angle) {
    // Nothing to do here
}


void Sphere::scale(float3 scaling) {
    this->radius *= scaling.x;  // Assuming uniform scaling for spheres
}
