#include "Mesh.hpp"


#include <array>
#include <fstream>
#include <iomanip>
#include <optional>
#include <sstream>
#include <utility>


namespace {

constexpr int MAX_BVH_DEPTH = 20;
constexpr float TRAVERSAL_COST = 1.0f;
constexpr float INTERSECTION_COST = 1.0f;


float centroidComponent(const Triangle& t, int ax) {
    switch (ax) {
        case 0: return (t.v0.x + t.v1.x + t.v2.x) / 3.0f;
        case 1: return (t.v0.y + t.v1.y + t.v2.y) / 3.0f;
        default: return (t.v0.z + t.v1.z + t.v2.z) / 3.0f;
    }
}


bool traverseBVH(const std::vector<FlatBVHNode>& flatNodes, const std::vector<Triangle>& triangles, int rootIndex, const Ray& ray, HitInfo& hit) {
    std::vector<int> stack;
    stack.reserve(64);
    stack.push_back(rootIndex); // root node is at the end of the vector

    bool result = false;

    while (!stack.empty()) {
        const int nodeIndex = stack.back();
        stack.pop_back();

        const FlatBVHNode& node = flatNodes[nodeIndex];

        const float dst = node.boundingBox.intersect(ray);
        if (dst >= hit.distance) {
            continue;
        }

        if (node.leftChildIndex == INVALID_NODE_INDEX) { // Leaf node
            for (uint32_t i = 0; i < node.triangleCount; ++i) {
                const Triangle& tri = triangles[node.triangleOffset + i];
                if (intersectOrientedTriangle(tri, ray, hit)) {
                    result = true;
                }
            }
        } else {
            // Push farther child first, so nearer is processed next
            const FlatBVHNode& left = flatNodes[node.leftChildIndex];
            const FlatBVHNode& right = flatNodes[node.rightChildIndex];

            const float distLeft = left.boundingBox.intersect(ray);
            const float distRight = right.boundingBox.intersect(ray);

            if (distLeft < distRight) {
                if (distRight < hit.distance)
                    stack.push_back(static_cast<int>(node.rightChildIndex));
                if (distLeft < hit.distance)
                    stack.push_back(static_cast<int>(node.leftChildIndex));
            } else {
                if (distLeft < hit.distance)
                    stack.push_back(static_cast<int>(node.leftChildIndex));
                if (distRight < hit.distance)
                    stack.push_back(static_cast<int>(node.rightChildIndex));
            }
        }
    }

    return result;
}


void translateBVH(std::vector<FlatBVHNode>& nodes, std::vector<Triangle>& triangles, float3 translation) {
    for (Triangle& tri : triangles) {
        tri.v0 = tri.v0 + translation;
        tri.v1 = tri.v1 + translation;
        tri.v2 = tri.v2 + translation;

        tri.edge1 = tri.v1 - tri.v0;
        tri.edge2 = tri.v2 - tri.v0;

        tri.normal = normalize(cross(tri.edge1, tri.edge2));
    }
    for (FlatBVHNode& node : nodes) {
        node.boundingBox.min = node.boundingBox.min + translation;
        node.boundingBox.max = node.boundingBox.max + translation;
    }
}


using RotationMatrix3 = std::array<std::array<float, 3>, 3>;


void rotateTriangle(Triangle& tri, const RotationMatrix3& matrix) {
    tri.v0 = float3{
        matrix[0][0] * tri.v0.x + matrix[0][1] * tri.v0.y + matrix[0][2] * tri.v0.z,
        matrix[1][0] * tri.v0.x + matrix[1][1] * tri.v0.y + matrix[1][2] * tri.v0.z,
        matrix[2][0] * tri.v0.x + matrix[2][1] * tri.v0.y + matrix[2][2] * tri.v0.z
    };
    tri.v1 = float3{
        matrix[0][0] * tri.v1.x + matrix[0][1] * tri.v1.y + matrix[0][2] * tri.v1.z,
        matrix[1][0] * tri.v1.x + matrix[1][1] * tri.v1.y + matrix[1][2] * tri.v1.z,
        matrix[2][0] * tri.v1.x + matrix[2][1] * tri.v1.y + matrix[2][2] * tri.v1.z
    };
    tri.v2 = float3{
        matrix[0][0] * tri.v2.x + matrix[0][1] * tri.v2.y + matrix[0][2] * tri.v2.z,
        matrix[1][0] * tri.v2.x + matrix[1][1] * tri.v2.y + matrix[1][2] * tri.v2.z,
        matrix[2][0] * tri.v2.x + matrix[2][1] * tri.v2.y + matrix[2][2] * tri.v2.z
    };

    tri.edge1 = tri.v1 - tri.v0;
    tri.edge2 = tri.v2 - tri.v0;

    tri.normal = normalize(cross(tri.edge1, tri.edge2));
}


RotationMatrix3 rotationMatrix(float3 axis, float angle) {
    const float rad = static_cast<float>(angle * PI / 180.0f);
    const float c = cos(rad);
    const float s = sin(rad);
    const float t = 1.0f - c;

    axis = normalize(axis);
    return {{
        {t * axis.x * axis.x + c, t * axis.x * axis.y - s * axis.z, t * axis.x * axis.z + s * axis.y},
        {t * axis.x * axis.y + s * axis.z, t * axis.y * axis.y + c, t * axis.y * axis.z - s * axis.x},
        {t * axis.x * axis.z - s * axis.y, t * axis.y * axis.z + s * axis.x, t * axis.z * axis.z + c}
    }};
}


AABB updateBoundsRecursive(std::vector<FlatBVHNode>& nodes, const std::vector<Triangle>& triangles, int nodeIndex) {
    FlatBVHNode& node = nodes[nodeIndex];
    if (node.leftChildIndex == INVALID_NODE_INDEX) { // Leaf node
        node.boundingBox = AABB();
        for (uint32_t i = 0; i < node.triangleCount; ++i) {
            const Triangle& tri = triangles[node.triangleOffset + i];
            node.boundingBox.expand(AABB::fromTriangle(tri));
        }
    } else {
        AABB leftBox = updateBoundsRecursive(nodes, triangles, static_cast<int>(node.leftChildIndex));
        AABB rightBox = updateBoundsRecursive(nodes, triangles, static_cast<int>(node.rightChildIndex));
        node.boundingBox = leftBox;
        node.boundingBox.expand(rightBox);
    }
    return node.boundingBox;
}


void rotateBVH(std::vector<FlatBVHNode>& nodes, std::vector<Triangle>& triangles, float3 axis, float angle) {
    // Rotate all triangles
    const RotationMatrix3 matrix = rotationMatrix(axis, angle);
    for (Triangle& tri : triangles) {
        rotateTriangle(tri, matrix);
    }

    updateBoundsRecursive(nodes, triangles, static_cast<int>(nodes.size()) - 1); // Start from the root node
}


void scaleBVH(std::vector<FlatBVHNode>& nodes, std::vector<Triangle>& triangles, float3 scaling) {
    // Scale triangles
    for (Triangle& tri : triangles) {
        tri.v0 = float3{tri.v0.x * scaling.x, tri.v0.y * scaling.y, tri.v0.z * scaling.z};
        tri.v1 = float3{tri.v1.x * scaling.x, tri.v1.y * scaling.y, tri.v1.z * scaling.z};
        tri.v2 = float3{tri.v2.x * scaling.x, tri.v2.y * scaling.y, tri.v2.z * scaling.z};

        tri.edge1 = tri.v1 - tri.v0;
        tri.edge2 = tri.v2 - tri.v0;

        tri.normal = normalize(cross(tri.edge1, tri.edge2));
    }

    // Scale bounding boxes
    for (FlatBVHNode& node : nodes) {
        node.boundingBox.min = float3{
            node.boundingBox.min.x * scaling.x,
            node.boundingBox.min.y * scaling.y,
            node.boundingBox.min.z * scaling.z
        };
        node.boundingBox.max = float3{
            node.boundingBox.max.x * scaling.x,
            node.boundingBox.max.y * scaling.y,
            node.boundingBox.max.z * scaling.z
        };
    }
}


struct BVHStats {
    int nodeCount = 0;
    int leafCount = 0;
    int minLeafDepth = std::numeric_limits<int>::max();
    int maxLeafDepth = 0;
    double sumLeafDepths = 0;
    int maxTrianglesInLeaf = 0;
    double sumTriangles = 0;
};


void collectBVHStats(const std::unique_ptr<BVHNode>& node, int depth, BVHStats& stats) {
    if (!node) return;

    stats.nodeCount++;

    if (node->isLeaf) {
        stats.leafCount++;
        stats.minLeafDepth = std::min(stats.minLeafDepth, depth);
        stats.maxLeafDepth = std::max(stats.maxLeafDepth, depth);
        stats.sumLeafDepths += depth;

        const int triangleCount = static_cast<int>(node->triangles.size());
        stats.maxTrianglesInLeaf = std::max(stats.maxTrianglesInLeaf, triangleCount);
        stats.sumTriangles += triangleCount;
    }

    collectBVHStats(node->left, depth + 1, stats);
    collectBVHStats(node->right, depth + 1, stats);
}

} // namespace


Mesh::Mesh(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open OBJ file\n";
        return;
    }

    std::vector<float3> vertices;
    std::vector<Triangle> triangles;

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
            std::array<int, 3> vi {};
            for (int i = 0; i < 3; ++i) {
                std::string vert;
                ss >> vert;
                std::istringstream vs(vert);
                std::string idxStr;
                std::getline(vs, idxStr, '/');           // Only take vertex index
                vi[i] = std::stoi(idxStr) - 1;           // OBJ indices start at 1
            }

            triangles.push_back(Triangle(vertices[vi[0]], vertices[vi[1]], vertices[vi[2]]));
        }
    }

    construct(triangles);
}


void Mesh::construct(std::vector<Triangle>& triangles) {
    std::cout << "Loaded " << triangles.size() << " triangles\n";

    std::unique_ptr<BVHNode> root = buildBVH(triangles);
    m_rootIndex = flattenBVH(root);
}


bool Mesh::intersect(const Ray& ray, HitInfo& globalHit) {
    if (m_nodes.empty()) {
        return false;
    }

    if (m_nodes.back().boundingBox.intersect(ray) >= FLOAT_MAX) { // root box test first
        return false;
    }

    HitInfo hit;
    if (traverseBVH(m_nodes, m_triangleArray, m_rootIndex, ray, hit)) {
        globalHit.distance = hit.distance;
        globalHit.position = hit.position;
        globalHit.normal = hit.normal;
        return true;
    } else {
        return false;
    }
}


void Mesh::translate(float3 translation) {
    translateBVH(m_nodes, m_triangleArray, translation);
    // updateAABBs();
}


void Mesh::rotate(float3 axis, float angle) {
    rotateBVH(m_nodes, m_triangleArray, axis, angle);
}


void Mesh::scale(float3 scaling) {
    scaleBVH(m_nodes, m_triangleArray, scaling);
}


void printStats(const std::unique_ptr<BVHNode>& root) {
    BVHStats stats;
    collectBVHStats(root, 0, stats);

    const double meanLeafDepth = stats.leafCount > 0 ? stats.sumLeafDepths / stats.leafCount : 0;
    const double meanTrianglesPerLeaf = stats.leafCount > 0 ? stats.sumTriangles / stats.leafCount : 0;

    std::cout << "BVH Statistics:\n"
              << "  Node Count: " << stats.nodeCount << "\n"
              << "  Leaf Node Count: " << stats.leafCount << "\n"
              << "  Leaf Depths:\n"
              << "    Min: " << stats.minLeafDepth << "\n"
              << "    Max: " << stats.maxLeafDepth << "\n"
              << "    Mean: " << std::fixed << std::setprecision(2) << meanLeafDepth << "\n"
              << "  Triangles in Leaf:\n"
              << "    Max: " << stats.maxTrianglesInLeaf << "\n"
              << "    Mean: " << std::fixed << std::setprecision(2) << meanTrianglesPerLeaf << "\n";
}


std::unique_ptr<BVHNode> Mesh::buildBVH(std::vector<Triangle>& triangles, int depth) {
    auto node = std::make_unique<BVHNode>();

    // Compute bounding box
    for (const auto& tri : triangles) {
        node->boundingBox.expand(AABB::fromTriangle(tri));
    }

    if (triangles.size() <= 2 || depth >= MAX_BVH_DEPTH) {
        node->isLeaf = true;
        node->triangles = triangles;
        return node;
    }

    const float parentArea = node->boundingBox.surfaceArea();

    int bestAxis = -1;
    std::optional<size_t> bestSplit;
    float bestCost = std::numeric_limits<float>::infinity();

    for (int axis = 0; axis < 3; ++axis) {
        // Sort triangles along axis by centroid
        std::sort(triangles.begin(), triangles.end(), [axis](const Triangle& a, const Triangle& b) {
            return centroidComponent(a, axis) < centroidComponent(b, axis);
        });

        // Precompute left and right bounding boxes
        std::vector<AABB> leftBoxes(triangles.size());
        std::vector<AABB> rightBoxes(triangles.size());

        AABB leftBox, rightBox;
        for (size_t i = 0; i < triangles.size(); ++i) {
            leftBox.expand(AABB::fromTriangle(triangles[i]));
            leftBoxes[i] = leftBox;
        }
        for (int i = static_cast<int>(triangles.size()) - 1; i >= 0; --i) {
            rightBox.expand(AABB::fromTriangle(triangles[i]));
            rightBoxes[static_cast<size_t>(i)] = rightBox;
        }

        // Try all possible split positions
        for (size_t i = 1; i < triangles.size(); ++i) {
            const float Sl = leftBoxes[i - 1].surfaceArea();
            const float Sr = rightBoxes[i].surfaceArea();

            const float cost = TRAVERSAL_COST + INTERSECTION_COST * (
                (Sl * i + Sr * (triangles.size() - i)) / parentArea
            );

            if (cost < bestCost) {
                bestCost = cost;
                bestAxis = axis;
                bestSplit = i;
            }
        }
    }

    // If no good split found, make leaf
    if (!bestSplit) {
        node->isLeaf = true;
        node->triangles = triangles;
        return node;
    }

    // Sort and split on best axis
    std::sort(triangles.begin(), triangles.end(), [bestAxis](const Triangle& a, const Triangle& b) {
        return centroidComponent(a, bestAxis) < centroidComponent(b, bestAxis);
    });

    const size_t split = *bestSplit;
    std::vector<Triangle> leftTriangles(triangles.begin(), triangles.begin() + static_cast<long>(split));
    std::vector<Triangle> rightTriangles(triangles.begin() + static_cast<long>(split), triangles.end());

    node->left = buildBVH(leftTriangles, depth + 1);
    node->right = buildBVH(rightTriangles, depth + 1);

    return node;
}


int Mesh::flattenBVH(const std::unique_ptr<BVHNode>& node) {
    if (!node) {
        return -1;
    }

    FlatBVHNode newNode;
    newNode.boundingBox = node->boundingBox;
    newNode.leftChildIndex = node->isLeaf ? INVALID_NODE_INDEX : 0;
    newNode.rightChildIndex = node->isLeaf ? INVALID_NODE_INDEX : 0;

    if (node->isLeaf) {
        newNode.triangleOffset = static_cast<uint32_t>(m_triangleArray.size());
        newNode.triangleCount = static_cast<uint32_t>(node->triangles.size());
        m_triangleArray.insert(m_triangleArray.end(),
                               node->triangles.begin(),
                               node->triangles.end());
    } else {
        // Recursively flatten children first
        newNode.leftChildIndex = static_cast<uint32_t>(flattenBVH(node->left));
        newNode.rightChildIndex = static_cast<uint32_t>(flattenBVH(node->right));
    }

    const int currentIndex = static_cast<int>(m_nodes.size());
    m_nodes.push_back(std::move(newNode)); // Only push AFTER children are ready

    return currentIndex;
}
