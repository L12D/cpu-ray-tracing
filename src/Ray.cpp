#include "Ray.hpp"
#include "Application.hpp"


Ray::Ray(float3 origin, float3 direction) {
    this->origin = origin;
    this->direction = direction;
    this->inverseDirection = {1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z};
    this->length = 10000.0;
    this->color = {0, 0, 0};
    this->hit = false;
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


float3 Ray::getInverseDirection() {
    return this->inverseDirection;
}


void Ray::setDirection(float3 direction) {
    this->direction = direction;
    this->inverseDirection = {1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z};
    if (this->inverseDirection.x == 0) this->inverseDirection.x = 1e6f; // Avoid division by zero
    if (this->inverseDirection.y == 0) this->inverseDirection.y = 1e6f; // Avoid division by zero
    if (this->inverseDirection.z == 0) this->inverseDirection.z = 1e6f; // Avoid division by zero
}


float Ray::getLength() {
    return this->length;
}


void Ray::setLength(float length) {
    this->length = length;
}


float3 Ray::getColor() {
    return this->color;
}


void Ray::setColor(float3 color) {
    this->color = color;
}


bool Ray::getHit() {
    return this->hit;
}


void Ray::setHit(bool hit) {
    this->hit = hit;
}


std::vector<Ray *> generateRays(float3 origin, float3 normal, float3 direction, int n) {
    std::vector<Ray *> rays;
    RandomGenerator *rng = Application::getInstance()->getRandomGenerator();

    for (int i = 0; i < n; ++i) {
        // Generate random angles
        float theta = 2.0f * M_PI * rng->next();  // Azimuthal angle [0, 2π]
        float phi = acos(2.0f * rng->next() - 1.0f);  // Polar angle [0, π]
        
        // Convert spherical to Cartesian coordinates
        float3 randomDirection = {
            sin(phi) * cos(theta),
            sin(phi) * sin(theta),
            cos(phi)
        };
        
        // Ensure the ray points in the same hemisphere as the normal
        if (dot(randomDirection, normal) < 0) {
            randomDirection = -randomDirection;
        }
        
        rays.push_back(new Ray(origin + mul(0.001f, normal), normalize(randomDirection)));
    }

    return rays;
}
