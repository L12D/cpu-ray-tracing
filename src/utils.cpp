#include "utils.hpp"


mat4 identity() {
    return {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}


mat4 mTranslate(float3 a) {
    return {
        {1, 0, 0, a.x},
        {0, 1, 0, a.y},
        {0, 0, 1, a.z},
        {0, 0, 0, 1}
    };
}


mat4 mRotate(float3 axis, float angle) {
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    const float t = 1 - c;
    const float3 n = normalize(axis);
    return {
        {t * n.x * n.x + c, t * n.x * n.y - s * n.z, t * n.x * n.z + s * n.y, 0},
        {t * n.x * n.y + s * n.z, t * n.y * n.y + c, t * n.y * n.z - s * n.x, 0},
        {t * n.x * n.z - s * n.y, t * n.y * n.z + s * n.x, t * n.z * n.z + c, 0},
        {0, 0, 0, 1}
    };
}


mat4 mScale(float3 a) {
    return {
        {a.x, 0, 0, 0},
        {0, a.y, 0, 0},
        {0, 0, a.z, 0},
        {0, 0, 0, 1}
    };
}


Ray generateRay(float3 origin, float3 normal, float3 /*direction*/) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Generate random angles
    const float theta = 2.0f * static_cast<float>(PI) * dis(gen); // Azimuthal angle [0, 2π]
    const float phi = acos(2.0f * dis(gen) - 1.0f);               // Polar angle [0, π]

    // Convert spherical to Cartesian coordinates
    float3 randomDirection = {
        static_cast<float>(sin(phi) * cos(theta)),
        static_cast<float>(sin(phi) * sin(theta)),
        static_cast<float>(cos(phi))
    };

    // Ensure the ray points in the same hemisphere as the normal
    if (dot(randomDirection, normal) < 0) {
        randomDirection = -randomDirection;
    }

    return Ray(origin + 0.001f * normal, normalize(randomDirection));
}


std::vector<Ray> generateRays(float3 origin, float3 normal, float3 /*direction*/) {
    std::vector<Ray> rays;
    rays.reserve(N_RAYS);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < N_RAYS; i++) {
        // Generate random angles
        const float theta = 2.0f * static_cast<float>(PI) * dis(gen); // Azimuthal angle [0, 2π]
        const float phi = acos(2.0f * dis(gen) - 1.0f);               // Polar angle [0, π]

        // Convert spherical to Cartesian coordinates
        float3 randomDirection = {
            static_cast<float>(sin(phi) * cos(theta)),
            static_cast<float>(sin(phi) * sin(theta)),
            static_cast<float>(cos(phi))
        };

        // Ensure the ray points in the same hemisphere as the normal
        if (dot(randomDirection, normal) < 0) {
            randomDirection = -randomDirection;
        }
        rays.push_back(Ray(origin + 0.0001f * normal, normalize(randomDirection)));
    }

    return rays;
}


Ray getMirrorRay(float3 intersectionPoint, float3 normal, float3 incident) {
    // Compute reflection direction using R = I - 2(N·I)N
    // where I is incident direction, N is normal
    const float3 reflectionDir = normalize(incident - (2.0f * dot(incident, normal)) * normal);

    // Add small offset to avoid self-intersection
    const float3 offsetOrigin = intersectionPoint + 0.0001f * normal;
    return Ray(offsetOrigin, reflectionDir);
}


float AABB::intersect(const Ray& ray) const noexcept {
    const float3 dirfrac = ray.inverseDirection;

    const float t1 = (min.x - ray.origin.x) * dirfrac.x;
    const float t2 = (max.x - ray.origin.x) * dirfrac.x;
    const float t3 = (min.y - ray.origin.y) * dirfrac.y;
    const float t4 = (max.y - ray.origin.y) * dirfrac.y;
    const float t5 = (min.z - ray.origin.z) * dirfrac.z;
    const float t6 = (max.z - ray.origin.z) * dirfrac.z;

    const float tmin = std::max({std::min(t1, t2), std::min(t3, t4), std::min(t5, t6)});
    const float tmax = std::min({std::max(t1, t2), std::max(t3, t4), std::max(t5, t6)});

    return (tmax < 0.0f || tmin > tmax) ? FLOAT_MAX : tmin;
}


AABB AABB::fromTriangle(const Triangle& tri) noexcept {
    const float3 min = {
        std::min({tri.v0.x, tri.v1.x, tri.v2.x}),
        std::min({tri.v0.y, tri.v1.y, tri.v2.y}),
        std::min({tri.v0.z, tri.v1.z, tri.v2.z})
    };

    const float3 max = {
        std::max({tri.v0.x, tri.v1.x, tri.v2.x}),
        std::max({tri.v0.y, tri.v1.y, tri.v2.y}),
        std::max({tri.v0.z, tri.v1.z, tri.v2.z})
    };

    return AABB{min, max};
}


bool intersectOrientedTriangle(const Triangle& tri, const Ray& ray, HitInfo& hit) {
    // Möller–Trumbore intersection test
    const float3& v0 = tri.v0;
    const float3& edge1 = tri.edge1;
    const float3& edge2 = tri.edge2;
    const float3& normal = tri.normal;

    if (dot(ray.direction, normal) >= 0) return false;

    const float3 h = cross(ray.direction, edge2);
    const float a = dot(edge1, h);
    if (std::abs(a) < 1e-6) return false;

    const float f = 1.0f / a;
    const float3 s = ray.origin - v0;
    const float u = f * dot(s, h);
    if (u < 0.0f || u > 1.0) return false;

    const float3 q = cross(s, edge1);
    const float v = f * dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0) return false;

    const float t = f * dot(edge2, q);
    if (t > 1e-6f && t < hit.distance) {
        hit.distance = t;
        hit.position = ray.origin + t * ray.direction;
        hit.normal = normal;
        return true;
    }

    return false;
}


bool intersectTriangle(const Triangle& tri, const Ray& ray, HitInfo& hit) {
    // Möller–Trumbore intersection test
    const float3& v0 = tri.v0;
    const float3& edge1 = tri.edge1;
    const float3& edge2 = tri.edge2;
    const float3& normal = tri.normal;

    const float3 h = cross(ray.direction, edge2);
    const float a = dot(edge1, h);
    if (std::abs(a) < 1e-6) return false;

    const float f = 1.0f / a;
    const float3 s = ray.origin - v0;
    const float u = f * dot(s, h);
    if (u < 0.0f || u > 1.0) return false;

    const float3 q = cross(s, edge1);
    const float v = f * dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0) return false;

    const float t = f * dot(edge2, q);
    if (t > 1e-6f && t < hit.distance) {
        hit.distance = t;
        hit.position = ray.origin + t * ray.direction;
        if (dot(normal, ray.direction) < 0.0) {
            hit.normal = normal;
        } else {
            hit.normal = -normal;
        }
        return true;
    }

    return false;
}
