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


void Object::intersect(const ray& ray, HitInfo &hit, int depth) {
    // return the distance to the intersection point

    if (depth == MAX_DEPTH) {
        hit.distance = FLOAT_MAX;
    }

    HitInfo shapeHit;
    if (!shape->intersect(ray, shapeHit)) {
        hit.distance = FLOAT_MAX;
    }
    hit.distance = shapeHit.distance;
    hit.position = shapeHit.position;
    hit.normal = shapeHit.normal;
}


float3 Object::getRayColor(float3 intersectionPoint, float3 normal, float3 incident, int depth) {
    Application* app = Application::getInstance();
    Scene* scene = app->getScene();

    if (depth == MAX_DEPTH) {
        return BC_COLOR_2;
    }

    if (isLight) {
        return color;
    }

    std::vector<ray> rays;
    if (depth == 0 && !isMirror) {

        // std::vector<float3> directions = app->getDirections();
        // for (float3& randomDirection : directions) {
        //     // Ensure the ray points in the same hemisphere as the normal
        //     if (dot(randomDirection, normal) < 0) {
        //         randomDirection = -randomDirection;
        //     }
        //     rays.push_back(ray(intersectionPoint + mul(0.001f, normal), normalize(randomDirection)));
        // }

        rays = generateRays(intersectionPoint, normal, incident, N_RAYS);

    } else {
        rays.push_back(getMirrorRay(intersectionPoint, normal, incident));
    }


    float3 reflexionColor = BC_COLOR_2;
    float rayLength;
    float3 rayColor;

    for (const ray& ray : rays) {
        rayLength = FLOAT_MAX;
        rayColor = BC_COLOR_2;

        HitInfo hit;
        Object *closestObject = nullptr;
        float3 position;
        float3 hitNormal;
        for (Object *object : scene->getObjects()) {
            object->intersect(ray, hit, depth);
            if (hit.distance < rayLength) {
                rayLength = hit.distance;
                position = hit.position;
                hitNormal = hit.normal;
                closestObject = object;
            }
        }

        if (closestObject != nullptr) {
            float3 direction = ray.direction;
            if (depth == 0 && isMirror) {
                rayColor = closestObject->getRayColor(position, hitNormal, direction, depth);
            } else {
                // For non-mirror objects, we can use the object's color directly
                rayColor = closestObject->getRayColor(position, hitNormal, direction, depth + 1);
                rayColor = mul(dot(direction, normal), rayColor);
            }
        }
        
        reflexionColor = reflexionColor + rayColor;
    }
    if (depth == 0 && !isMirror) {
        reflexionColor = mul(INV_N_RAYS, reflexionColor);
    }
    return reflexionColor.clamp()*color;
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
