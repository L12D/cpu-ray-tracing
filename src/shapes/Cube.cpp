#include "Cube.hpp"


Cube::Cube() : Cube({-0.5, -0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}) {}


Cube::Cube(float3 base, float3 v1, float3 v2, float3 v3) {
    std::vector<Triangle> triangles;

    triangles.push_back(Triangle(base + v1, base, base + v2));
    triangles.push_back(Triangle(base + v1 + v2, base + v1, base + v2));

    triangles.push_back(Triangle(base + v2, base, base + v3));
    triangles.push_back(Triangle(base + v2 + v3, base + v2, base + v3));

    triangles.push_back(Triangle(base + v3, base, base + v1));
    triangles.push_back(Triangle(base + v3 + v1, base + v3, base + v1));

    triangles.push_back(Triangle(base + v3, base + v1 + v3, base + v2 + v3));
    triangles.push_back(Triangle(base + v1 + v3, base + v1 + v2 + v3, base + v2 + v3));

    triangles.push_back(Triangle(base + v1, base + v2 + v1, base + v3 + v1));
    triangles.push_back(Triangle(base + v2 + v1, base + v2 + v3 + v1, base + v3 + v1));

    triangles.push_back(Triangle(base + v2, base + v3 + v2, base + v1 + v2));
    triangles.push_back(Triangle(base + v3 + v2, base + v3 + v1 + v2, base + v1 + v2));

    m_mesh.construct(triangles);
}


bool Cube::intersect(const Ray& ray, HitInfo& globalHit) {
    return m_mesh.intersect(ray, globalHit);
}


void Cube::translate(float3 translation) {
    m_mesh.translate(translation);
}


void Cube::rotate(float3 axis, float angle) {
    m_mesh.rotate(axis, angle);
}


void Cube::scale(float3 scaling) {
    m_mesh.scale(scaling);
}
