#include "Object.hpp"


Object::Object() {
    this->shape = nullptr;
    this->bounding_box = std::make_pair((float3){0, 0, 0}, (float3){0, 0, 0});
}


Object::Object(Shape *shape) {
    this->shape = shape;
    this->bounding_box = this->computeBoundingBox();
}


Shape *Object::getShape() {
    return this->shape;
}


void Object::setShape(Shape *shape) {
    this->shape = shape;
    this->bounding_box = this->computeBoundingBox();
}


std::pair<float3, float3> Object::computeBoundingBox() {
    return std::make_pair((float3){0, 0, 0}, (float3){0, 0, 0});
}


std::pair<float3, float3> Object::getBoundingBox() {
    return this->bounding_box;
}


Object::~Object() {
    delete this->shape;
}
