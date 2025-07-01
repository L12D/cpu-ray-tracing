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
    std::vector<triangle> triangles;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back({x, y, z});
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
            triangles.push_back(tri);
        }
    }
    root = buildBVH(triangles);

    std::cout << "Loaded " << triangles.size() << " triangles.\n";
    printStats();
}


bool traverseBVH(const std::unique_ptr<BVHNode>& node, Ray *ray, HitInfo& hit) {
    if (!node || !node->boundingBox.intersect(ray))
        return false;

    bool result = false;

    if (node->isLeaf && node->triangles) {
        for (const auto& tri : *(node->triangles)) {
            const float3& v0 = tri.v0;
            const float3& v1 = tri.v1;
            const float3& v2 = tri.v2;

            float3 edge1 = v1 - v0;
            float3 edge2 = v2 - v0;
            float3 h = cross(ray->getDirection(), edge2);
            float a = dot(edge1, h);

            if (std::abs(a) < 1e-6f) continue; // Ray is parallel

            float f = 1.0f / a;
            float3 s = ray->getOrigin() - v0;
            float u = f * dot(s, h);
            if (u < 0.0f || u > 1.0f) continue;

            float3 q = cross(s, edge1);
            float v = f * dot(ray->getDirection(), q);
            if (v < 0.0f || (u + v) > 1.0f) continue;

            float t = f * dot(edge2, q);
            if (t > 1e-6f && t < hit.distance) {
                hit.distance = t;
                hit.position = ray->getOrigin() + mul(t, ray->getDirection());
                hit.normal = normalize(cross(edge1, edge2));
                result = true;
            }
        }
    } else {
        bool hitLeft = traverseBVH(node->left, ray, hit);
        bool hitRight = traverseBVH(node->right, ray, hit);
        result = hitLeft || hitRight;
    }

    return result;
}


bool TriangleSet::intersect(Ray *ray, HitInfo& globalHit) {
    if (!root->boundingBox.intersect(ray)) {
        return false;
    }

    HitInfo hit;
    if (traverseBVH(root, ray, hit)) {
        globalHit.distance = hit.distance;
        globalHit.position = hit.position;
        globalHit.normal = hit.normal;
        return true;
    } else {
        return false;
    }
}


void translateBVH(std::unique_ptr<BVHNode>& node, float3 translation) {
    if (!node) return;

    if (node->isLeaf && node->triangles) {
        for (auto& tri : *(node->triangles)) {
            tri.v0 = tri.v0 + translation;
            tri.v1 = tri.v1 + translation;
            tri.v2 = tri.v2 + translation;
        }
    }

    translateBVH(node->left, translation);
    translateBVH(node->right, translation);

    // Update bounding box after translating children
    node->boundingBox.min = node->boundingBox.min + translation;
    node->boundingBox.max = node->boundingBox.max + translation;
}


void TriangleSet::translate(float3 translation) {
    translateBVH(root, translation);
}


void rotateBVH(std::unique_ptr<BVHNode>& node, float3 axis, float angle) {
    if (!node) return;

    // Convert angle to radians and prepare rotation matrix
    float rad = angle * M_PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);
    float t = 1.0f - c;
    
    axis = normalize(axis);
    float matrix[3][3] = {
        {t*axis.x*axis.x + c, t*axis.x*axis.y - s*axis.z, t*axis.x*axis.z + s*axis.y},
        {t*axis.x*axis.y + s*axis.z, t*axis.y*axis.y + c, t*axis.y*axis.z - s*axis.x},
        {t*axis.x*axis.z - s*axis.y, t*axis.y*axis.z + s*axis.x, t*axis.z*axis.z + c}
    };

    if (node->isLeaf && node->triangles) {
        for (auto& tri : *(node->triangles)) {
            // Rotate each vertex
            tri.v0 = float3{
                matrix[0][0]*tri.v0.x + matrix[0][1]*tri.v0.y + matrix[0][2]*tri.v0.z,
                matrix[1][0]*tri.v0.x + matrix[1][1]*tri.v0.y + matrix[1][2]*tri.v0.z,
                matrix[2][0]*tri.v0.x + matrix[2][1]*tri.v0.y + matrix[2][2]*tri.v0.z
            };
            tri.v1 = float3{
                matrix[0][0]*tri.v1.x + matrix[0][1]*tri.v1.y + matrix[0][2]*tri.v1.z,
                matrix[1][0]*tri.v1.x + matrix[1][1]*tri.v1.y + matrix[1][2]*tri.v1.z,
                matrix[2][0]*tri.v1.x + matrix[2][1]*tri.v1.y + matrix[2][2]*tri.v1.z
            };
            tri.v2 = float3{
                matrix[0][0]*tri.v2.x + matrix[0][1]*tri.v2.y + matrix[0][2]*tri.v2.z,
                matrix[1][0]*tri.v2.x + matrix[1][1]*tri.v2.y + matrix[1][2]*tri.v2.z,
                matrix[2][0]*tri.v2.x + matrix[2][1]*tri.v2.y + matrix[2][2]*tri.v2.z
            };
        }
    }

    // Recursively rotate children
    rotateBVH(node->left, axis, angle);
    rotateBVH(node->right, axis, angle);

    // Update bounding box
    node->boundingBox = AABB();
    if (node->isLeaf && node->triangles) {
        for (const auto& tri : *(node->triangles)) {
            node->boundingBox.expand(AABB::fromTriangle(tri));
        }
    } else {
        if (node->left) node->boundingBox.expand(node->left->boundingBox);
        if (node->right) node->boundingBox.expand(node->right->boundingBox);
    }
}


void TriangleSet::rotate(float3 axis, float angle) {
    rotateBVH(root, axis, angle);
}


void scaleBVH(std::unique_ptr<BVHNode>& node, float3 scaling) {
    if (!node) return;

    if (node->isLeaf && node->triangles) {
        for (auto& tri : *(node->triangles)) {
            tri.v0 = scaling * tri.v0;
            tri.v1 = scaling * tri.v1;
            tri.v2 = scaling * tri.v2;
        }
    }

    scaleBVH(node->left, scaling);
    scaleBVH(node->right, scaling);

    // Update bounding box after scaling children
    node->boundingBox.min = scaling * node->boundingBox.min;
    node->boundingBox.max = scaling * node->boundingBox.max;
}


void TriangleSet::scale(float3 scaling) {
    scaleBVH(root, scaling);
}


void collectBVHStats(const std::unique_ptr<BVHNode>& node, int depth,
                    int& nodeCount, int& leafCount,
                    int& minLeafDepth, int& maxLeafDepth, double& sumLeafDepths,
                    int& maxTrianglesInLeaf, double& sumTriangles) {
    if (!node) return;

    nodeCount++;

    if (node->isLeaf) {
        leafCount++;
        minLeafDepth = std::min(minLeafDepth, depth);
        maxLeafDepth = std::max(maxLeafDepth, depth);
        sumLeafDepths += depth;
        
        int triangleCount = node->triangles ? node->triangles->size() : 0;
        maxTrianglesInLeaf = std::max(maxTrianglesInLeaf, triangleCount);
        sumTriangles += triangleCount;
    }

    collectBVHStats(node->left, depth + 1, 
                   nodeCount, leafCount,
                   minLeafDepth, maxLeafDepth, sumLeafDepths,
                   maxTrianglesInLeaf, sumTriangles);
    collectBVHStats(node->right, depth + 1,
                   nodeCount, leafCount,
                   minLeafDepth, maxLeafDepth, sumLeafDepths,
                   maxTrianglesInLeaf, sumTriangles);
}


void TriangleSet::printStats() {
    int nodeCount = 0;
    int leafCount = 0;
    int minLeafDepth = std::numeric_limits<int>::max();
    int maxLeafDepth = 0;
    double sumLeafDepths = 0;
    int maxTrianglesInLeaf = 0;
    double sumTriangles = 0;

    collectBVHStats(root, 0,
                   nodeCount, leafCount,
                   minLeafDepth, maxLeafDepth, sumLeafDepths,
                   maxTrianglesInLeaf, sumTriangles);

    double meanLeafDepth = leafCount > 0 ? sumLeafDepths / leafCount : 0;
    double meanTrianglesPerLeaf = leafCount > 0 ? sumTriangles / leafCount : 0;

    std::cout << "BVH Statistics:\n"
              << "  Node Count: " << nodeCount << "\n"
              << "  Leaf Node Count: " << leafCount << "\n"
              << "  Leaf Depths:\n"
              << "    Min: " << minLeafDepth << "\n"
              << "    Max: " << maxLeafDepth << "\n"
              << "    Mean: " << std::fixed << std::setprecision(2) << meanLeafDepth << "\n"
              << "  Triangles in Leaf:\n"
              << "    Max: " << maxTrianglesInLeaf << "\n"
              << "    Mean: " << std::fixed << std::setprecision(2) << meanTrianglesPerLeaf << "\n";
}


std::unique_ptr<BVHNode> TriangleSet::buildBVH(std::vector<triangle>& triangles, int depth) {
    auto node = std::make_unique<BVHNode>();

    // Compute bounding box
    for (const auto& tri : triangles) {
        AABB triBox = AABB::fromTriangle(tri);
        if (node->boundingBox.min.x == 0 && node->boundingBox.max.x == 0) {
            node->boundingBox = triBox;
        } else {
            node->boundingBox.expand(triBox);
        }
    }

    if (triangles.size() <= 2 || depth > 20) {
        node->isLeaf = true;
        node->triangles = std::make_unique<std::vector<triangle>>(triangles);
        return node;
    }

    // Split by longest axis
    float3 size = node->boundingBox.max - node->boundingBox.min;
    int axis = (size.x > size.y && size.x > size.z) ? 0 : (size.y > size.z ? 1 : 2);

    std::sort(triangles.begin(), triangles.end(), [axis](const triangle& a, const triangle& b) {
        float ac = (axis == 0 ? (a.v0.x + a.v1.x + a.v2.x) : 
                   axis == 1 ? (a.v0.y + a.v1.y + a.v2.y) : 
                              (a.v0.z + a.v1.z + a.v2.z)) / 3.0f;
        float bc = (axis == 0 ? (b.v0.x + b.v1.x + b.v2.x) : 
                   axis == 1 ? (b.v0.y + b.v1.y + b.v2.y) : 
                              (b.v0.z + b.v1.z + b.v2.z)) / 3.0f;
        return ac < bc;
    });

    size_t mid = triangles.size() / 2;
    std::vector<triangle> leftTriangles(triangles.begin(), triangles.begin() + mid);
    std::vector<triangle> rightTriangles(triangles.begin() + mid, triangles.end());

    node->left = buildBVH(leftTriangles, depth + 1);
    node->right = buildBVH(rightTriangles, depth + 1);

    return node;
}