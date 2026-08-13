#include "Object.hpp"


Object::Object() {
    this->shape = nullptr;
    this->color = {0, 0, 0};
    this->light = false;
    this->mirror = false;
    this->invisible = false;
}


Object::Object(Shape *shape, float3 color) {
    this->shape = shape;
    this->color = color;
    this->light = false;
    this->mirror = false;
    this->invisible = false;
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


void Object::setLight(bool light) {
    this->light = light;
}


bool Object::isLight() {
    return this->light;
}


void Object::setMirror(bool mirror) {
    this->mirror = mirror;
}


bool Object::isMirror() {
    return this->mirror;
}


void Object::setInvisible(bool invisible) {
    this->invisible = invisible;
}


bool Object::isInvisible() {
    return this->invisible;
}


void Object::intersect(const ray& ray, HitInfo &hit) {
    // return the distance to the intersection point

    if (hit.actualDepth == MAX_ACTUAL_DEPTH) {
        hit.distance = FLOAT_MAX;
        return;
    }

    HitInfo shapeHit;
    if (!shape->intersect(ray, shapeHit)) {
        hit.distance = FLOAT_MAX;
        return;
    }
    hit.distance = shapeHit.distance;
    hit.position = shapeHit.position;
    hit.normal = shapeHit.normal;
}


float3 Object::getRayColor(float3 intersectionPoint, float3 normal, float3 incident, int renderDepth, int actualDepth) {
    if (actualDepth >= MAX_ACTUAL_DEPTH) {
        return BC_COLOR_2;
    }

    Application* app = Application::getInstance();
    Scene* scene = app->getScene();

    if (light) {
        return color;
    }

    HitInfo hit;

    std::vector<ray> rays;
    if (mirror) {
        rays.push_back(getMirrorRay(intersectionPoint, normal, incident));

        hit.renderDepth = renderDepth;
        hit.actualDepth = actualDepth + 1;
    } else {
        if (renderDepth == 0) {
            if (PRODUCTION) {
                std::vector<float3> directions = app->getDirections();
                for (float3& randomDirection : directions) {
                    // Ensure the ray points in the same hemisphere as the normal
                    if (dot(randomDirection, normal) < 0) {
                        randomDirection = -randomDirection;
                    }
                    rays.push_back(ray(intersectionPoint + mul(0.0001f, normal), normalize(randomDirection)));
                }
            } else {
                rays = generateRays(intersectionPoint, normal, incident);
            }
        } else {
            rays.push_back(generateRay(intersectionPoint, normal, incident));
        }

        hit.renderDepth = renderDepth + 1;
        hit.actualDepth = actualDepth + 1;
    }

    if (hit.renderDepth >= MAX_RENDER_DEPTH) {
        return BC_COLOR_2;
    }


    float3 reflexionColor = float3();
    float rayLength;
    float3 rayColor;

    for (const ray& ray : rays) {
        rayLength = FLOAT_MAX;
        rayColor = BC_COLOR_2;

        Object *closestObject = nullptr;
        float3 position;
        float3 hitNormal;
        for (Object *object : scene->getObjects()) {
            object->intersect(ray, hit);
            if (hit.distance < rayLength) {
                rayLength = hit.distance;
                position = hit.position;
                hitNormal = hit.normal;
                closestObject = object;
            }
        }

        if (closestObject != nullptr) {
            float3 direction = ray.direction;
            rayColor = closestObject->getRayColor(position, hitNormal, direction, hit.renderDepth, hit.actualDepth);
            if (!mirror) {
                rayColor = mul(dot(direction, normal), rayColor);
            }
        }
        
        reflexionColor = reflexionColor + rayColor;
    }
    if (!mirror) {
        if (renderDepth == 0) {
            reflexionColor = mul(INV_N_RAYS, reflexionColor);
        }
        reflexionColor = reflexionColor + (float3)BC_COLOR_2;
    } else {
        reflexionColor = mul(MIRROR_REFLECTIVENESS, reflexionColor);
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
