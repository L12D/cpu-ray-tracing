#include "Object.hpp"


Object::Object() {
    this->shape = nullptr;
    this->color = {0, 0, 0};
    this->isLight = false;
    this->isMirror = false;
}


Object::Object(Shape *shape, float3 color) {
    this->shape = shape;
    this->color = color;
    this->isLight = false;
    this->isMirror = false;
}


Shape *Object::getShape() {
    return this->shape;
}


void Object::setShape(Shape *shape) {
    this->shape = shape;
}


float3 Object::getColor() {
    return this->color;
}


void Object::setColor(float3 color) {
    this->color = color;
}


void Object::setLight() {
    this->isLight = true;
}


void Object::setMirror() {
    this->isMirror = true;
}


float Object::intersect(Ray *ray, int depth, int maxDepth) {
    // return the distance to the intersection point
    Scene* scene = Application::getInstance()->getScene();
    float brightness = scene->getBrightness();
    float3 backgroundColor = scene->getBackgroundColor();

    if (depth == maxDepth) {
        ray->setColor(backgroundColor);
        return std::numeric_limits<float>::max();
    }

    HitInfo hit;
    if (!shape->intersect(ray, hit)) {
        ray->setColor(backgroundColor);
        return std::numeric_limits<float>::max();
    }
    if (isLight) {
        ray->setColor(color);
        return hit.distance;
    }

    std::vector<Ray *> rays;
    if (depth == 0 && !isMirror) {
        rays = generateRays(hit.position, hit.normal, ray->getDirection(), 6000);
    } else {
        rays.push_back(getMirrorRay(ray, hit.position, hit.normal));
    }

    float3 reflexionColor = backgroundColor;
    float rayLength;
    float3 rayColor;
    
    for (Ray *r : rays) {
        rayLength = std::numeric_limits<float>::max();
        rayColor = backgroundColor;
        for (Object *object : scene->getObjects()) {
            float distance;
            if (depth == 0 && isMirror) {
                distance = object->intersect(r, depth, maxDepth);
            } else {
                distance = object->intersect(r, depth+1, maxDepth);
            }
            if (distance < rayLength) {
                rayLength = distance;
                rayColor = mul(dot(r->getDirection(), hit.normal), r->getColor());
            }
        }
        reflexionColor = reflexionColor + mul(brightness, rayColor);
    }
    reflexionColor = mul(1.0f/rays.size(), reflexionColor);
    reflexionColor.x = std::min(1.0f, std::max(0.0f, reflexionColor.x));
    reflexionColor.y = std::min(1.0f, std::max(0.0f, reflexionColor.y));
    reflexionColor.z = std::min(1.0f, std::max(0.0f, reflexionColor.z));

    ray->setColor(reflexionColor*this->color);
    for (Ray *r : rays) {
        delete r;
    }
    
    return hit.distance;
}


void Object::translate(float3 translation) {
    this->shape->translate(translation);
}


void Object::rotate(float3 axis, float angle) {
    this->shape->rotate(axis, angle);
}


void Object::scale(float3 scaling) {
    this->shape->scale(scaling);
}


Object::~Object() {
    delete this->shape;
}
