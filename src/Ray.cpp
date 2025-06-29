#include "Ray.hpp"
#include "Application.hpp"


Ray::Ray(float3 origin, float3 direction) {
    this->origin = origin;
    this->direction = direction;
    this->inverseDirection = {1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z};
    this->color = {0, 0, 0};
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


float3 Ray::getColor() {
    return this->color;
}


void Ray::setColor(float3 color) {
    this->color = color;
}


std::vector<Ray *> generateRays(float3 origin, float3 normal, float3 direction, int n) {
    std::vector<Ray *> rays;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < n; ++i) {
        // Generate random angles
        float theta = 2.0f * M_PI * dis(gen);  // Azimuthal angle [0, 2π]
        float phi = acos(2.0f * dis(gen) - 1.0f);  // Polar angle [0, π]
        
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


Ray *getMirrorRay(Ray *ray, float3 intersectionPoint, float3 normal) {
    // Compute reflection direction using R = I - 2(N·I)N
    // where I is incident direction, N is normal
    float3 incident = ray->getDirection();
    float3 reflectionDir = normalize(incident - mul(2.0f * dot(incident, normal), normal));
    
    // Add small offset to avoid self-intersection
    float3 offsetOrigin = intersectionPoint + mul(0.001f, normal);
    return new Ray(offsetOrigin, reflectionDir);
}
