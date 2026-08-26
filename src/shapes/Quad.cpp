#include "Quad.hpp"


namespace {

void translateTriangle(Triangle& tri, const float3& translation) {
    tri.v0 = tri.v0 + translation;
    tri.v1 = tri.v1 + translation;
    tri.v2 = tri.v2 + translation;

    tri.edge1 = tri.v1 - tri.v0;
    tri.edge2 = tri.v2 - tri.v0;

    tri.normal = normalize(cross(tri.edge1, tri.edge2));
}

} // namespace


Quad::Quad() : Quad({-0.5, 0.0, -0.5}, {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}) {}


Quad::Quad(float3 base, float3 diagonal, float3 v1, float3 v2)
    : m_tri1(base, base + diagonal, base + v1), m_tri2(base, base + diagonal, base + v2) {}


bool Quad::intersect(const Ray& ray, HitInfo& globalHit) {
    HitInfo closestHit;
    bool foundHit = false;

    if (intersectTriangle(m_tri1, ray, closestHit)) {
        foundHit = true;
    }

    HitInfo secondHit;
    if (intersectTriangle(m_tri2, ray, secondHit) && secondHit.distance < closestHit.distance) {
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
    translateTriangle(m_tri1, translation);
    translateTriangle(m_tri2, translation);
}


void Quad::rotate(float3 /*axis*/, float /*angle*/) {
    // TODO
}


void Quad::scale(float3 /*scaling*/) {
    // TODO
}
