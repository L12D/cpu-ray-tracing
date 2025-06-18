#include "Ray.hpp"
#include "Application.hpp"


Ray::Ray(float3 origin, float3 direction) {
    this->origin = origin;
    this->direction = direction;
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


void Ray::setDirection(float3 direction) {
    this->direction = direction;
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
