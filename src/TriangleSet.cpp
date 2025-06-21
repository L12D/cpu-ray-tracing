#include "TriangleSet.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


TriangleSet::TriangleSet(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open OBJ file.\n";
        return;
    }

    std::vector<float3> vertices;
    triangles = new std::vector<triangle>();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back({x, y+8, z-4});
        } else if (type == "f") {
            int vi[3];
            for (int i = 0; i < 3; ++i) {
                std::string vert;
                ss >> vert;
                std::istringstream vs(vert);
                std::string idxStr;
                std::getline(vs, idxStr, '/'); // Only take vertex index
                vi[i] = std::stoi(idxStr) - 1; // OBJ indices start at 1
            }

            triangle tri(vertices[vi[0]], vertices[vi[1]], vertices[vi[2]]);
            triangles->push_back(tri);
        }
    }
    computeBoundingBox();

    std::cout << "Loaded " << triangles->size() << " triangles.\n";
}


void TriangleSet::computeBoundingBox() {
    if (triangles->empty()) {
        boundingBox = {float3{0, 0, 0}, float3{0, 0, 0}};
        return;
    }

    float3 min = triangles->at(0).v0;
    float3 max = min;

    for (const auto& tri : *triangles) {
        // Check all vertices of the triangle
        min.x = std::min({min.x, tri.v0.x, tri.v1.x, tri.v2.x});
        min.y = std::min({min.y, tri.v0.y, tri.v1.y, tri.v2.y});
        min.z = std::min({min.z, tri.v0.z, tri.v1.z, tri.v2.z});

        max.x = std::max({max.x, tri.v0.x, tri.v1.x, tri.v2.x});
        max.y = std::max({max.y, tri.v0.y, tri.v1.y, tri.v2.y});
        max.z = std::max({max.z, tri.v0.z, tri.v1.z, tri.v2.z});
    }

    boundingBox = {min, max};
}


std::vector<triangle>* TriangleSet::getTriangles() {
    return triangles;
}


void TriangleSet::setTriangles(std::vector<triangle>* triangles) {
    this->triangles = triangles;
}


std::pair<float3, float3> TriangleSet::intersect(Ray* ray) {
    // Skip intersection test if ray doesn't intersect bounding box
    float3 dirfrac = ray->getInverseDirection();

    float t1 = (boundingBox.first.x - ray->getOrigin().x) * dirfrac.x;
    float t2 = (boundingBox.second.x - ray->getOrigin().x) * dirfrac.x;
    float t3 = (boundingBox.first.y - ray->getOrigin().y) * dirfrac.y;
    float t4 = (boundingBox.second.y - ray->getOrigin().y) * dirfrac.y;
    float t5 = (boundingBox.first.z - ray->getOrigin().z) * dirfrac.z;
    float t6 = (boundingBox.second.z - ray->getOrigin().z) * dirfrac.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) {
        ray->setHit(false);
        return {{0, 0, 0}, {0, 0, 0}};
    }
    

    float3 closestIntersection = {0, 0, 0};
    float3 normal = {0, 0, 0};
    float closestDistance = std::numeric_limits<float>::max();
    bool hit = false;

    for (const auto& tri : *triangles) {
        const float3& v0 = tri.v0;
        const float3& v1 = tri.v1;
        const float3& v2 = tri.v2;
        
        float3 edge1 = v1 - v0;
        float3 edge2 = v2 - v0;
        float3 h = cross(ray->getDirection(), edge2);
        float a = dot(edge1, h);

        if (std::abs(a) < 1e-6f)
            continue; // Ray is parallel to triangle

        float f = 1.0f / a;
        float3 s = ray->getOrigin() - v0;
        float u = f * dot(s, h);

        if (u < 0.0f || u > 1.0f)
            continue;

        float3 q = cross(s, edge1);
        float v = f * dot(ray->getDirection(), q);

        if (v < 0.0f || u + v > 1.0f)
            continue;

        float t = f * dot(edge2, q);

        if (t > 1e-6f && t < closestDistance) {
            closestDistance = t;
            closestIntersection = ray->getOrigin() + mul(t, ray->getDirection());
            normal = normalize(cross(edge1, edge2));
            hit = true;
        }
    }

    if (hit) {
        ray->setHit(true);
        ray->setLength(closestDistance);
        return {closestIntersection, normal};
    } else {
        ray->setHit(false);
        return {{0, 0, 0}, {0, 0, 0}};
    }
}


TriangleSet::~TriangleSet() {
    delete triangles;
}