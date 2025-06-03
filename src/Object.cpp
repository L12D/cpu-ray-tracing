#include "Object.hpp"


Object::Object() {
    this->shape = nullptr;
    this->color = {0, 0, 0};
    this->bounding_box = std::make_pair((float3){0, 0, 0}, (float3){0, 0, 0});
}


Object::Object(Shape *shape, float3 color, bool isLight) {
    this->shape = shape;
    this->color = color;
    this->isLight = isLight;
    this->bounding_box = this->computeBoundingBox();
}


Shape *Object::getShape() {
    return this->shape;
}


void Object::setShape(Shape *shape) {
    this->shape = shape;
    this->bounding_box = this->computeBoundingBox();
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


std::pair<float3, float3> Object::computeBoundingBox() {
    return std::make_pair((float3){0, 0, 0}, (float3){0, 0, 0});
}


std::pair<float3, float3> Object::getBoundingBox() {
    return this->bounding_box;
}


void Object::intersect(Ray *ray, int depth) {
    if (depth == 0) {
        ray->setColor({0.2, 0.2, 0.2});
        return;
    }
    // TODO : Add bounding box test
    std::pair<float3, float3> pair = shape->intersect(ray, depth);
    if (!ray->getHit()) {
        ray->setColor({0.2, 0.2, 0.2});
        return;
    }
    if (isLight) {
        // std::cout << "Light at depth " << depth << std::endl;
        ray->setColor(color);
        return;
    }
    float3 intersectionPoint = pair.first;
    float3 normal = pair.second;
    // std::vector<Ray *> rays = generateRays(intersectionPoint, normal, ray->getDirection(), 500);
    std::vector<Ray *> rays;

    // Compute reflection direction using R = I - 2(N·I)N
    // where I is incident direction, N is normal
    float3 incident = ray->getDirection();
    float3 reflectionDir = normalize(incident - mul(2.0f * dot(incident, normal), normal));
    
    // Add small offset to avoid self-intersection
    float3 offsetOrigin = intersectionPoint + mul(0.001f, normal);
    rays.push_back(new Ray(offsetOrigin, reflectionDir));
    
    float3 reflexionColor = {0, 0, 0};
    Scene* scene = Scene::getInstance();
    float rayLength;
    float3 rayColor;
    // std::cout << "Depth: " << depth << std::endl;
    for (Ray *r : rays) {
        rayLength = 10000.0;
        rayColor = {0.2, 0.2, 0.2};
        for (Object *object : scene->getObjects()) {
            object->intersect(r, depth-1);
            // std::cout << "Ray " << r->getLength() << ", Direction: (" << r->getDirection().x << ", " << r->getDirection().y << ", " << r->getDirection().z << "), Origin: (" << r->getOrigin().x << ", " << r->getOrigin().y << ", " << r->getOrigin().z << ")" << std::endl;
            if (r->getLength() < rayLength) {
                // std::cout << "Intersecting with object at depth " << depth-1 << std::endl;
                rayLength = r->getLength();
                rayColor = r->getColor();
            }
        }
        // std::cout << "Ray color: (" << r->getColor().x << ", " << r->getColor().y << ", " << r->getColor().z << ")" << std::endl;
        reflexionColor = reflexionColor + rayColor;
    }
    // std::cout << "Reflexion color: (" << reflexionColor.x << ", " << reflexionColor.y << ", " << reflexionColor.z << ")" << std::endl;
    reflexionColor = mul(1/rays.size(), reflexionColor);
    ray->setColor(reflexionColor*color);
    for (Ray *r : rays) {
        delete r;
    }
    
}


Object::~Object() {
    delete this->shape;
}
