#include "Object.hpp"


Object::Object() {
    this->shape = nullptr;
    this->color = {0, 0, 0};
}


Object::Object(Shape *shape, float3 color) {
    this->shape = shape;
    this->color = color;
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


void Object::intersect(Ray *ray, int depth, int maxDepth) {
    Scene* scene = Application::getInstance()->getScene();
    float brightness = scene->getBrightness();
    float3 backgroundColor = scene->getBackgroundColor();

    if (depth == maxDepth) {
        ray->setColor(backgroundColor);
        return;
    }

    std::pair<float3, float3> pair = shape->intersect(ray);
    if (!ray->getHit()) {
        ray->setColor(backgroundColor);
        return;
    }
    if (isLight) {
        ray->setColor(color);
        return;
    }

    float3 intersectionPoint = pair.first;
    float3 normal = pair.second;

    std::vector<Ray *> rays;
    if (depth == 0 && !isMirror) {
        rays = generateRays(intersectionPoint, normal, ray->getDirection(), 1500);
    } else {
        rays.push_back(getMirrorRay(ray, intersectionPoint, normal));
    }

    float3 reflexionColor = backgroundColor;
    float rayLength;
    float3 rayColor;
    
    for (Ray *r : rays) {
        rayLength = 10000.0;
        rayColor = backgroundColor;
        for (Object *object : scene->getObjects()) {
            if (depth == 0 && isMirror) {
                object->intersect(r, depth, maxDepth);
            } else {
                object->intersect(r, depth+1, maxDepth);
            }
            if (r->getLength() < rayLength) {
                rayLength = r->getLength();
                rayColor = r->getColor();
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
