#include "Mesh.hpp"



Mesh::Mesh(std::string name) {
    if (name == "lion") {
        this->set = new TriangleSet("assets/lion3.obj");
        this->partialSet = new TriangleSet("assets/lion05.obj");
    }
}



bool Mesh::intersect(const ray& ray, HitInfo& globalHit) {
    // TODO: select the set to compute the intersection
    return partialSet->intersect(ray, globalHit);
}


void Mesh::translate(float3 translation) {
    set->translate(translation);
    partialSet->translate(translation);
}


void Mesh::rotate(float3 axis, float angle) {
    set->rotate(axis, angle);
    partialSet->rotate(axis, angle);
}


void Mesh::scale(float3 scaling) {
    set->scale(scaling);
    partialSet->scale(scaling);
}


Mesh::~Mesh() {
    delete this->set;
    delete this->partialSet;
}
