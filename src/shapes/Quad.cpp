#include "Quad.hpp"


Quad::Quad(): Quad({-0.5, 0.0, -0.5}, {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}) {}


Quad::Quad(float3 base, float3 diagonal, float3 v1, float3 v2)
    : tri1(base, base + diagonal, base + v1), tri2(base, base + diagonal, base + v2) {}


bool Quad::intersect(const ray& ray, HitInfo& globalHit) {
    HitInfo closestHit;
    closestHit.distance = FLOAT_MAX;
    bool foundHit = false;

    if (intersectTriangle(tri1, ray, closestHit)) {
        foundHit = true;
    }

    HitInfo secondHit;
    secondHit.distance = FLOAT_MAX;
    if (intersectTriangle(tri2, ray, secondHit) && secondHit.distance < closestHit.distance) {
        closestHit = secondHit;
        foundHit = true;
    }

    if (!foundHit) {
        globalHit.distance = FLOAT_MAX;
        return false;
    }

    globalHit = closestHit;
    return true;
}


void Quad::translate(float3 translation) {
    tri1.v0 = tri1.v0 + translation;
    tri1.v1 = tri1.v1 + translation;
    tri1.v2 = tri1.v2 + translation;

    tri1.edge1 = tri1.v1 - tri1.v0;
    tri1.edge2 = tri1.v2 - tri1.v0;

    tri1.normal = normalize(cross(tri1.edge1, tri1.edge2));

    tri2.v0 = tri2.v0 + translation;
    tri2.v1 = tri2.v1 + translation;
    tri2.v2 = tri2.v2 + translation;

    tri2.edge1 = tri2.v1 - tri2.v0;
    tri2.edge2 = tri2.v2 - tri2.v0;

    tri2.normal = normalize(cross(tri2.edge1, tri2.edge2));
}


void Quad::rotate(float3 axis, float angle) {
    // TODO
}


void Quad::scale(float3 scaling) {
    // TODO
}
