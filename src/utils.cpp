#include "utils.hpp"


mat4 id() {
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
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1 - c;
    float3 n = normalize(axis);
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


float3 mul(mat4 a, float3 b) {
    return {
        dot(a.row0, {b.x, b.y, b.z, 1}),
        dot(a.row1, {b.x, b.y, b.z, 1}),
        dot(a.row2, {b.x, b.y, b.z, 1})
    };
}


float dot(float3 a, float3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


float3 cross(float3 a, float3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}


float3 mul(float a, float3 b) {
    return {a * b.x, a * b.y, a * b.z};
}


float dot(float4 a, float4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


float3 normalize(float3 a) {
    float length = std::sqrt(dot(a, a));
    return {a.x / length, a.y / length, a.z / length};
}


ray generateRay(float3 origin, float3 normal, float3 direction) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

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
    
    return ray(origin + mul(0.001f, normal), normalize(randomDirection));
}


std::vector<ray> generateRays(float3 origin, float3 normal, float3 direction) {
    std::vector<ray> rays;
    rays.reserve(N_RAYS);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < N_RAYS; i++) {
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
        rays.push_back(ray(origin + mul(0.0001f, normal), normalize(randomDirection)));
    }
    
    return rays;
}


ray getMirrorRay(float3 intersectionPoint, float3 normal, float3 incident) {
    // Compute reflection direction using R = I - 2(N·I)N
    // where I is incident direction, N is normal
    float3 reflectionDir = normalize(incident - mul(2.0f * dot(incident, normal), normal));
    
    // Add small offset to avoid self-intersection
    float3 offsetOrigin = intersectionPoint + mul(0.0001f, normal);
    return ray(offsetOrigin, reflectionDir);
}


float AABB::intersect(const ray& ray) const {
    float3 dirfrac = ray.inverseDirection;

    float t1 = (min.x - ray.origin.x) * dirfrac.x;
    float t2 = (max.x - ray.origin.x) * dirfrac.x;
    float t3 = (min.y - ray.origin.y) * dirfrac.y;
    float t4 = (max.y - ray.origin.y) * dirfrac.y;
    float t5 = (min.z - ray.origin.z) * dirfrac.z;
    float t6 = (max.z - ray.origin.z) * dirfrac.z;

    // float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    // float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    float tmin = (((t1 < t2 ? t1 : t2) > (t3 < t4 ? t3 : t4) ? (t1 < t2 ? t1 : t2) : (t3 < t4 ? t3 : t4)) > (t5 < t6 ? t5 : t6) 
              ? ((t1 < t2 ? t1 : t2) > (t3 < t4 ? t3 : t4) ? (t1 < t2 ? t1 : t2) : (t3 < t4 ? t3 : t4)) 
              : (t5 < t6 ? t5 : t6));

    float tmax = (((t1 > t2 ? t1 : t2) < (t3 > t4 ? t3 : t4) ? (t1 > t2 ? t1 : t2) : (t3 > t4 ? t3 : t4)) < (t5 > t6 ? t5 : t6) 
              ? ((t1 > t2 ? t1 : t2) < (t3 > t4 ? t3 : t4) ? (t1 > t2 ? t1 : t2) : (t3 > t4 ? t3 : t4)) 
              : (t5 > t6 ? t5 : t6));


    // if (tmax < 0 || tmin > tmax) {
    //     return FLOAT_MAX;
    // }
    // return tmin;

    return (tmax < 0.0f || tmin > tmax) ? FLOAT_MAX : tmin;
}


AABB AABB::fromTriangle(const triangle& tri) {
    // float3 min = {
    //     std::min(std::min(tri.v0.x, tri.v1.x), tri.v2.x),
    //     std::min(std::min(tri.v0.y, tri.v1.y), tri.v2.y),
    //     std::min(std::min(tri.v0.z, tri.v1.z), tri.v2.z)
    // };

    // float3 max = {
    //     std::max(std::max(tri.v0.x, tri.v1.x), tri.v2.x),
    //     std::max(std::max(tri.v0.y, tri.v1.y), tri.v2.y),
    //     std::max(std::max(tri.v0.z, tri.v1.z), tri.v2.z)
    // };

    float3 min = {
        (tri.v0.x < tri.v1.x ? (tri.v0.x < tri.v2.x ? tri.v0.x : tri.v2.x) : (tri.v1.x < tri.v2.x ? tri.v1.x : tri.v2.x)),
        (tri.v0.y < tri.v1.y ? (tri.v0.y < tri.v2.y ? tri.v0.y : tri.v2.y) : (tri.v1.y < tri.v2.y ? tri.v1.y : tri.v2.y)),
        (tri.v0.z < tri.v1.z ? (tri.v0.z < tri.v2.z ? tri.v0.z : tri.v2.z) : (tri.v1.z < tri.v2.z ? tri.v1.z : tri.v2.z))
    };

    float3 max = {
        (tri.v0.x > tri.v1.x ? (tri.v0.x > tri.v2.x ? tri.v0.x : tri.v2.x) : (tri.v1.x > tri.v2.x ? tri.v1.x : tri.v2.x)),
        (tri.v0.y > tri.v1.y ? (tri.v0.y > tri.v2.y ? tri.v0.y : tri.v2.y) : (tri.v1.y > tri.v2.y ? tri.v1.y : tri.v2.y)),
        (tri.v0.z > tri.v1.z ? (tri.v0.z > tri.v2.z ? tri.v0.z : tri.v2.z) : (tri.v1.z > tri.v2.z ? tri.v1.z : tri.v2.z))
    };

    return AABB{min, max};
}


bool intersect(const triangle &tri, const ray &ray, HitInfo &hit, bool &result) {
    // Möller–Trumbore intersection test
    const float3 &v0 = tri.v0;
    const float3 &edge1 = tri.edge1;
    const float3 &edge2 = tri.edge2;
    const float3 &normal = tri.normal;

    if (dot(ray.direction, normal) >= 0) return false;

    float3 h = cross(ray.direction, edge2);
    float a = dot(edge1, h);
    if (std::abs(a) < 1e-6) return false;

    float f = 1.0f / a;
    float3 s = ray.origin - v0;
    float u = f * dot(s, h);
    if (u < 0.0f || u > 1.0) return false;

    float3 q = cross(s, edge1);
    float v = f * dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0) return false;

    float t = f * dot(edge2, q);
    if (t > 1e-6f && t < hit.distance) {
        hit.distance = t;
        hit.position = ray.origin + mul(t, ray.direction);
        hit.normal = normal;
        return true;
    }

    return false;
}