#include "Object.hpp"

#include <utility>


Object::Object(std::unique_ptr<Shape> shape, float3 color)
    : m_shape(std::move(shape)),
      m_color(color) {}


Shape* Object::getShape() const noexcept {
    return m_shape.get();
}


void Object::setShape(std::unique_ptr<Shape> shape) noexcept {
    m_shape = std::move(shape);
}


float3 Object::getColor() const noexcept {
    return m_color;
}


void Object::setColor(float3 color) noexcept {
    m_color = color;
}


void Object::setLight(bool light) noexcept {
    m_light = light;
}


bool Object::isLight() const noexcept {
    return m_light;
}


void Object::setMirror(bool mirror) noexcept {
    m_mirror = mirror;
}


bool Object::isMirror() const noexcept {
    return m_mirror;
}


void Object::setInvisible(bool invisible) noexcept {
    m_invisible = invisible;
}


bool Object::isInvisible() const noexcept {
    return m_invisible;
}


void Object::intersect(const Ray& ray, HitInfo& hit) {
    // return the distance to the intersection point

    if (hit.actualDepth == MAX_ACTUAL_DEPTH) {
        hit.distance = FLOAT_MAX;
        return;
    }

    HitInfo shapeHit;
    if (!m_shape->intersect(ray, shapeHit)) {
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

    Application& app = Application::getInstance();
    Scene* scene = &app.getScene();

    if (m_light) {
        return m_color;
    }

    HitInfo hit;

    std::vector<Ray> rays;
    if (m_mirror) {
        rays.push_back(getMirrorRay(intersectionPoint, normal, incident));

        hit.renderDepth = renderDepth;
        hit.actualDepth = actualDepth + 1;
    } else {
        if (renderDepth == 0) {
            if constexpr (PRODUCTION) {
                std::vector<float3> directions = app.getDirections();
                for (float3& randomDirection : directions) {
                    // Ensure the ray points in the same hemisphere as the normal
                    if (dot(randomDirection, normal) < 0) {
                        randomDirection = -randomDirection;
                    }
                    rays.push_back(Ray(intersectionPoint + 0.0001f * normal, normalize(randomDirection)));
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


    float3 reflexionColor;
    float rayLength;
    float3 rayColor;

    for (const Ray& ray : rays) {
        rayLength = FLOAT_MAX;
        rayColor = BC_COLOR_2;

        Object* closestObject = nullptr;
        float3 position;
        float3 hitNormal;
        for (const auto& object : scene->getObjects()) {
            object->intersect(ray, hit);
            if (hit.distance < rayLength) {
                rayLength = hit.distance;
                position = hit.position;
                hitNormal = hit.normal;
                closestObject = object.get();
            }
        }

        if (closestObject != nullptr) {
            const float3 direction = ray.direction;
            rayColor = closestObject->getRayColor(position, hitNormal, direction, hit.renderDepth, hit.actualDepth);
            if (!m_mirror) {
                rayColor = dot(direction, normal) * rayColor;
            }
        }

        reflexionColor = reflexionColor + rayColor;
    }
    if (!m_mirror) {
        if (renderDepth == 0) {
            reflexionColor = INV_N_RAYS * reflexionColor;
        }
        reflexionColor = reflexionColor + BC_COLOR_2;
    } else {
        reflexionColor = MIRROR_REFLECTIVENESS * reflexionColor;
    }
    return reflexionColor.clamp() * m_color;
}


void Object::translate(float3 translation) {
    m_shape->translate(translation);
}


void Object::rotate(float3 axis, float angle) {
    m_shape->rotate(axis, angle);
}


void Object::scale(float3 scaling) {
    m_shape->scale(scaling);
}
