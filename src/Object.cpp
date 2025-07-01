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


void Object::intersect(Ray *ray, HitInfo &hit, int depth, int maxDepth) {
    // return the distance to the intersection point
    
    if (depth == maxDepth) {
        hit.distance = std::numeric_limits<float>::max();
    }

    HitInfo shapeHit;
    if (!shape->intersect(ray, shapeHit)) {
        hit.distance = std::numeric_limits<float>::max();
    }
    hit.distance = shapeHit.distance;
    hit.position = shapeHit.position;
    hit.normal = shapeHit.normal;
}


float3 Object::getRayColor(float3 intersectionPoint, float3 normal, float3 incident, int depth, int maxDepth) {
    if (isLight) {
        return color;
    }

    std::vector<Ray *> rays;
    if (depth == 0 && !isMirror) {
        rays = generateRays(intersectionPoint, normal, incident, 200);
    } else {
        rays.push_back(getMirrorRay(intersectionPoint, normal, incident));
    }

    Scene* scene = Application::getInstance()->getScene();
    float3 backgroundColor = scene->getBackgroundColor();

    float3 reflexionColor = backgroundColor;
    float rayLength;
    float3 rayColor;
    
    for (Ray *ray : rays) {
        rayLength = std::numeric_limits<float>::max();
        rayColor = backgroundColor;

        HitInfo hit;
        Object *closestObject = nullptr;
        float3 position;
        float3 hitNormal;
        for (Object *object : scene->getObjects()) {
            object->intersect(ray, hit, depth, maxDepth);
            if (hit.distance < rayLength) {
                rayLength = hit.distance;
                position = hit.position;
                hitNormal = hit.normal;
                closestObject = object;
            }
        }

        if (closestObject != nullptr) {
            float3 direction = ray->getDirection();
            if (depth == 0 && isMirror) {
                rayColor = closestObject->getRayColor(position, hitNormal, direction, depth, maxDepth);
            } else {
                // For non-mirror objects, we can use the object's color directly
                rayColor = closestObject->getRayColor(position, hitNormal, direction, depth + 1, maxDepth);
                rayColor = mul(dot(direction, normal), rayColor);
            }
        }
        
        reflexionColor = reflexionColor + rayColor;
    }
    reflexionColor = mul(1.0f/rays.size(), reflexionColor);
    reflexionColor.x = std::min(1.0f, std::max(0.0f, reflexionColor.x));
    reflexionColor.y = std::min(1.0f, std::max(0.0f, reflexionColor.y));
    reflexionColor.z = std::min(1.0f, std::max(0.0f, reflexionColor.z));

    for (Ray *r : rays) {
        delete r;
    }
    return reflexionColor*color;
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
