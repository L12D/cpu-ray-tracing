#include "Object.hpp"


Object::Object() {
    this->shape = nullptr;
    this->color = {0, 0, 0};
}


Object::Object(Shape *shape, float3 color, bool isLight) {
    this->shape = shape;
    this->color = color;
    this->isLight = isLight;
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


bool Object::getIsLight() {
    return this->isLight;
}


void Object::setIsLight(bool isLight) {
    this->isLight = isLight;
}


void Object::intersect(Ray *ray, int depth, int maxDepth) {
    Scene* scene = Application::getInstance()->getScene();
    float brightness = scene->getBrightness();
    float3 backgroundColor = scene->getBackgroundColor();

    if (depth == maxDepth) {
        ray->setColor(backgroundColor);
        return;
    }
    // TODO : Add bounding box test
    std::pair<float3, float3> pair = shape->intersect(ray);
    if (!ray->getHit()) {
        ray->setColor(backgroundColor);
        return;
    }
    if (isLight) {
        // if (depth == 1) {
        //     std::cout << "Light at depth " << depth << std::endl;
        // }
        ray->setColor(color);
        return;
    }
    float3 intersectionPoint = pair.first;
    float3 normal = pair.second;
    int n; // Number of rays to generate for reflection
    if (depth == 0) {
        n = 60;
    } else {
        n = 10; // Generate 10 reflection rays otherwise
    }
    std::vector<Ray *> rays = generateRays(intersectionPoint, normal, ray->getDirection(), n);

    // std::vector<Ray *> rays;

    // // Compute reflection direction using R = I - 2(N·I)N
    // // where I is incident direction, N is normal
    // float3 incident = ray->getDirection();
    // float3 reflectionDir = normalize(incident - mul(2.0f * dot(incident, normal), normal));
    
    // // Add small offset to avoid self-intersection
    // float3 offsetOrigin = intersectionPoint + mul(0.001f, normal);
    // rays.push_back(new Ray(offsetOrigin, reflectionDir));

    float3 reflexionColor = backgroundColor;
    float rayLength;
    float3 rayColor;
    // std::cout << "Depth: " << depth << std::endl;
    for (Ray *r : rays) {
        rayLength = 10000.0;
        rayColor = backgroundColor;
        for (Object *object : scene->getObjects()) {
            object->intersect(r, depth+1, maxDepth);
            // std::cout << "Ray " << r->getLength() << ", Direction: (" << r->getDirection().x << ", " << r->getDirection().y << ", " << r->getDirection().z << "), Origin: (" << r->getOrigin().x << ", " << r->getOrigin().y << ", " << r->getOrigin().z << ")" << std::endl;
            if (r->getLength() < rayLength) {
                // std::cout << "Intersecting with object at depth " << depth-1 << std::endl;
                rayLength = r->getLength();
                rayColor = r->getColor();
            }
        }
        // std::cout << "Ray color: (" << r->getColor().x << ", " << r->getColor().y << ", " << r->getColor().z << ")" << std::endl;
        reflexionColor = reflexionColor + mul(brightness, rayColor);
    }
    // if (depth == 2) {
    //     std::cout << "Reflexion color: (" << reflexionColor.x << ", " << reflexionColor.y << ", " << reflexionColor.z << ")" << std::endl;
    // }
    reflexionColor = mul(1.0f/rays.size(), reflexionColor);
    reflexionColor.x = std::min(1.0f, std::max(0.0f, reflexionColor.x));
    reflexionColor.y = std::min(1.0f, std::max(0.0f, reflexionColor.y));
    reflexionColor.z = std::min(1.0f, std::max(0.0f, reflexionColor.z));

    ray->setColor(reflexionColor*this->color);
    for (Ray *r : rays) {
        delete r;
    }
    
}


Object::~Object() {
    delete this->shape;
}
