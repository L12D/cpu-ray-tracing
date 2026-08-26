#include "Sphere.hpp"


Sphere::Sphere(float3 center, float radius)
    : m_center(center),
      m_radius(radius) {}


float3 Sphere::getCenter() const noexcept {
    return m_center;
}


void Sphere::setCenter(float3 center) noexcept {
    m_center = center;
}


float Sphere::getRadius() const noexcept {
    return m_radius;
}


void Sphere::setRadius(float radius) noexcept {
    m_radius = radius;
}


bool Sphere::intersect(const Ray& ray, HitInfo& hit) {
    const float3 dir = ray.direction;
    const float3 oc = ray.origin - m_center;
    const float b = 2.0f * dot(oc, dir);
    const float c = dot(oc, oc) - m_radius * m_radius;

    const float discriminant = b * b - 4.0f * c; // Since a = 1

    if (discriminant < 0.0f) {
        return false;
    }

    const float sqrtDiscriminant = sqrt(discriminant);

    const float t0 = (-b - sqrtDiscriminant) * 0.5f; // Dividing by 2 directly
    const float t1 = (-b + sqrtDiscriminant) * 0.5f;

    // Return the closest valid intersection point
    if (t0 > 0.0) {
        hit.distance = t0;
        hit.position = ray.origin + t0 * ray.direction;
        hit.normal = normalize(hit.position - m_center);
        return true;
    } else if (t1 > 0.0) {
        hit.distance = t1;
        hit.position = ray.origin + t1 * ray.direction;
        hit.normal = normalize(hit.position - m_center);
        return true;
    }

    return false;
}


void Sphere::translate(float3 translation) {
    m_center = m_center + translation;
}


void Sphere::rotate(float3 /*axis*/, float /*angle*/) {
    // Nothing to do here
}


void Sphere::scale(float3 scaling) {
    m_radius *= scaling.x; // Assuming uniform scaling for spheres
}
