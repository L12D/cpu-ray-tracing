#include "Mesh.hpp"


Mesh::Mesh() {}


Mesh::Mesh(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open OBJ file\n";
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

    construct(triangles);
}


void Mesh::construct(std::vector<triangle> &triangles) {
    std::cout << "Loaded " << triangles.size() << " triangles\n";

    std::unique_ptr<BVHNode> root = buildBVH(triangles);
    rootIndex = flattenBVH(root);
}


bool traverseBVH(const std::vector<FlatBVHNode>& flatNodes, const std::vector<triangle>& triangles, int rootIndex, const ray& ray, HitInfo& hit) {
    std::vector<int> stack;
    stack.reserve(64);
    stack.push_back(rootIndex); // root node is at the end of the vector

    bool result = false;

    while (!stack.empty()) {
        int nodeIndex = stack.back();
        stack.pop_back();

        const FlatBVHNode& node = flatNodes[nodeIndex];

        float dst = node.boundingBox.intersect(ray);
        if (dst >= hit.distance) {
            continue;
        }

        if (node.leftChildIndex == 4294967295) { // Leaf node
            for (uint32_t i = 0; i < node.triangleCount; ++i) {
                const triangle& tri = triangles[node.triangleOffset + i];
                if (intersect(tri, ray, hit, result)) {
                    result = true;
                }
            }
        } else {
            // Push farther child first, so nearer is processed next
            const FlatBVHNode& left = flatNodes[node.leftChildIndex];
            const FlatBVHNode& right = flatNodes[node.rightChildIndex];

            float distLeft = left.boundingBox.intersect(ray);
            float distRight = right.boundingBox.intersect(ray);

            if (distLeft < distRight) {
                if (distRight < hit.distance)
                    stack.push_back(node.rightChildIndex);
                if (distLeft < hit.distance)
                    stack.push_back(node.leftChildIndex);
            } else {
                if (distLeft < hit.distance)
                    stack.push_back(node.leftChildIndex);
                if (distRight < hit.distance)
                    stack.push_back(node.rightChildIndex);
            }
        }
    }

    return result;
}


bool Mesh::intersect(const ray& ray, HitInfo& globalHit) {
    if (nodes.back().boundingBox.intersect(ray) >= FLOAT_MAX) {
        return false;
    }

    HitInfo hit;
    hit.distance = FLOAT_MAX;
    if (traverseBVH(nodes, triangleArray, rootIndex, ray, hit)) {
        globalHit.distance = hit.distance;
        globalHit.position = hit.position;
        globalHit.normal = hit.normal;
        return true;
    } else {
        return false;
    }
}


void translateBVH(std::vector<FlatBVHNode>& nodes, std::vector<triangle>& triangles, float3 translation) {
    for (size_t i = 0; i < triangles.size(); ++i) {
        triangles[i].v0 = triangles[i].v0 + translation;
        triangles[i].v1 = triangles[i].v1 + translation;
        triangles[i].v2 = triangles[i].v2 + translation;

        triangles[i].edge1 = triangles[i].v1 - triangles[i].v0;
        triangles[i].edge2 = triangles[i].v2 - triangles[i].v0;

        triangles[i].normal = normalize(cross(triangles[i].edge1, triangles[i].edge2));
    }
    for (FlatBVHNode& node : nodes) {
        node.boundingBox.min = node.boundingBox.min + translation;
        node.boundingBox.max = node.boundingBox.max + translation;
    }
}


void Mesh::translate(float3 translation) {
    translateBVH(nodes, triangleArray, translation);
    // updateAABBs();
}


void rotateBVH(std::vector<FlatBVHNode>& nodes, std::vector<triangle>& triangles, float3 axis, float angle) {
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

    // Rotate all triangles
    for (triangle& tri : triangles) {
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

        tri.edge1 = tri.v1 - tri.v0;
        tri.edge2 = tri.v2 - tri.v0;

        tri.normal = normalize(cross(tri.edge1, tri.edge2));
    }

    std::function<AABB(int)> updateBounds = [&](int nodeIndex) -> AABB {
        FlatBVHNode& node = nodes[nodeIndex];
        if (node.leftChildIndex == 4294967295) { // Leaf node
            node.boundingBox = AABB();
            for (uint32_t i = 0; i < node.triangleCount; ++i) {
                const triangle& tri = triangles[node.triangleOffset + i];
                node.boundingBox.expand(AABB::fromTriangle(tri));
            }
        } else {
            AABB leftBox = updateBounds(node.leftChildIndex);
            AABB rightBox = updateBounds(node.rightChildIndex);
            node.boundingBox = leftBox;
            node.boundingBox.expand(rightBox);
        }
        return node.boundingBox;
    };
    updateBounds(nodes.size() - 1); // Start from the root node
}


void Mesh::rotate(float3 axis, float angle) {
    rotateBVH(nodes, triangleArray, axis, angle);
}


void scaleBVH(std::vector<FlatBVHNode>& node, std::vector<triangle>& triangles, float3 scaling) {
    // Scale triangles
    for (triangle& tri : triangles) {
        tri.v0 = float3{tri.v0.x * scaling.x, tri.v0.y * scaling.y, tri.v0.z * scaling.z};
        tri.v1 = float3{tri.v1.x * scaling.x, tri.v1.y * scaling.y, tri.v1.z * scaling.z};
        tri.v2 = float3{tri.v2.x * scaling.x, tri.v2.y * scaling.y, tri.v2.z * scaling.z};

        tri.edge1 = tri.v1 - tri.v0;
        tri.edge2 = tri.v2 - tri.v0;

        tri.normal = normalize(cross(tri.edge1, tri.edge2));
    }

    // Scale bounding boxes
    for (FlatBVHNode& node : node) {
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


void Mesh::scale(float3 scaling) {
    scaleBVH(nodes, triangleArray, scaling);
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


void printStats(const std::unique_ptr<BVHNode>& root) {
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


std::unique_ptr<BVHNode> Mesh::buildBVH(std::vector<triangle>& triangles, int depth) {
    auto node = std::make_unique<BVHNode>();

    // Compute bounding box
    for (const auto& tri : triangles) {
        AABB triBox = AABB::fromTriangle(tri);
        node->boundingBox.expand(triBox);
    }

    if (triangles.size() <= 2 || depth >= 20) {
        node->isLeaf = true;
        node->triangles = std::make_unique<std::vector<triangle>>(triangles);
        return node;
    }

    const float traversalCost = 1.0f;
    const float intersectionCost = 1.0f;
    const float parentArea = node->boundingBox.surfaceArea();

    int bestAxis = -1;
    int bestSplit = -1;
    float bestCost = std::numeric_limits<float>::infinity();

    for (int axis = 0; axis < 3; ++axis) {
        // Sort triangles along axis by centroid
        std::sort(triangles.begin(), triangles.end(), [axis](const triangle& a, const triangle& b) {
            auto centroid = [](const triangle& t, int ax) {
            float v0c = (ax == 0) ? t.v0.x : (ax == 1) ? t.v0.y : t.v0.z;
            float v1c = (ax == 0) ? t.v1.x : (ax == 1) ? t.v1.y : t.v1.z;
            float v2c = (ax == 0) ? t.v2.x : (ax == 1) ? t.v2.y : t.v2.z;
            return (v0c + v1c + v2c) / 3.0f;
        };
            return centroid(a, axis) < centroid(b, axis);
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
            rightBoxes[i] = rightBox;
        }

        // Try all possible split positions
        for (size_t i = 1; i < triangles.size(); ++i) {
            float Sl = leftBoxes[i - 1].surfaceArea();
            float Sr = rightBoxes[i].surfaceArea();

            float cost = traversalCost + intersectionCost * (
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
    if (bestSplit == -1) {
        node->isLeaf = true;
        node->triangles = std::make_unique<std::vector<triangle>>(triangles);
        return node;
    }

    // Sort and split on best axis
    std::sort(triangles.begin(), triangles.end(), [bestAxis](const triangle& a, const triangle& b) {
        auto centroid = [](const triangle& t, int ax) {
            float v0c = (ax == 0) ? t.v0.x : (ax == 1) ? t.v0.y : t.v0.z;
            float v1c = (ax == 0) ? t.v1.x : (ax == 1) ? t.v1.y : t.v1.z;
            float v2c = (ax == 0) ? t.v2.x : (ax == 1) ? t.v2.y : t.v2.z;
            return (v0c + v1c + v2c) / 3.0f;
        };
        return centroid(a, bestAxis) < centroid(b, bestAxis);
    });

    std::vector<triangle> leftTriangles(triangles.begin(), triangles.begin() + bestSplit);
    std::vector<triangle> rightTriangles(triangles.begin() + bestSplit, triangles.end());

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
    newNode.leftChildIndex = node->isLeaf ? 4294967295 : 0;
    newNode.rightChildIndex = node->isLeaf ? 4294967295 : 0;

    if (node->isLeaf) {
        newNode.triangleOffset = triangleArray.size();
        newNode.triangleCount = node->triangles ? node->triangles->size() : 0;
        if (node->triangles) {
            triangleArray.insert(triangleArray.end(),
                                 node->triangles->begin(),
                                 node->triangles->end());
        }
    } else {
        // Recursively flatten children first
        int leftIndex = flattenBVH(node->left);
        int rightIndex = flattenBVH(node->right);
        newNode.leftChildIndex = leftIndex;
        newNode.rightChildIndex = rightIndex;
    }

    int currentIndex = nodes.size();
    nodes.push_back(std::move(newNode)); // Only push AFTER children are ready

    return currentIndex;
}
