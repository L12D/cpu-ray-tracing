#include "Cube.hpp"


Cube::Cube() : Cube({-0.5, -0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}) {}


Cube::Cube(float3 base, float3 v1, float3 v2, float3 v3) {
    this->mesh = new Mesh();

    std::vector<triangle> triangles;

    triangles.push_back(triangle(base + v1, base, base + v2));
    triangles.push_back(triangle(base + v1 + v2, base + v1, base + v2));

    triangles.push_back(triangle(base + v2, base, base + v3));
    triangles.push_back(triangle(base + v2 + v3, base + v2, base + v3));

    triangles.push_back(triangle(base + v3, base, base + v1));
    triangles.push_back(triangle(base + v3 + v1, base + v3, base + v1));

    triangles.push_back(triangle(base + v3, base + v1 + v3, base + v2 + v3));
    triangles.push_back(triangle(base + v1 + v3, base + v1 + v2 + v3, base + v2 + v3));

    triangles.push_back(triangle(base + v1, base + v2 + v1, base + v3 + v1));
    triangles.push_back(triangle(base + v2 + v1, base + v2 + v3 + v1, base + v3 + v1));

    triangles.push_back(triangle(base + v2, base + v3 + v2, base + v1 + v2));
    triangles.push_back(triangle(base + v3 + v2, base + v3 + v1 + v2, base + v1 + v2));

    mesh->construct(triangles);
}


bool Cube::intersect(const ray& ray, HitInfo& globalHit) {
    return this->mesh->intersect(ray, globalHit);
}


void Cube::translate(float3 translation) {
    this->mesh->translate(translation);
}


void Cube::rotate(float3 axis, float angle) {
    this->mesh->rotate(axis, angle);
}


void Cube::scale(float3 scaling) {
    this->mesh->scale(scaling);
}


Cube::~Cube() {
    delete this->mesh;
}
