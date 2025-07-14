#pragma once
#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

template <typename T>
class Octree {
   public:
    struct Node {
        glm::vec3 center;
        float halfSize;
        std::vector<T> data;
        std::array<std::unique_ptr<Node>, 8> children;

        bool isLeaf() const { return !children[0]; }
    };

    Octree(const glm::vec3& center, float halfSize, int maxDepth = 8)
        : maxDepth(maxDepth), currentDepth(0) {
        root = std::make_unique<Node>();
        root->center = center;
        root->halfSize = halfSize;
    }

    void insert(const T& item, const glm::vec3& position) {
        insertHelper(root.get(), item, position, 0);
    }

    std::vector<T> query(const glm::vec3& min, const glm::vec3& max) const {
        std::vector<T> results;
        queryHelper(root.get(), min, max, results);
        return results;
    }

    const Node* getRoot() const { return root.get(); }

   private:
    std::unique_ptr<Node> root;
    int maxDepth;
    int currentDepth;

    void insertHelper(Node* node, const T& item, const glm::vec3& position,
                      int depth) {
        if (!node) return;

        // Check if point is inside this node
        glm::vec3 diff = glm::abs(position - node->center);
        if (diff.x > node->halfSize || diff.y > node->halfSize ||
            diff.z > node->halfSize) {
            return;  // Point is outside this node
        }

        // If leaf node or max depth reached, add item here
        if (depth >= maxDepth || (node->isLeaf() && node->data.size() < 8)) {
            node->data.push_back(item);
            return;
        }

        // If leaf but full, subdivide
        if (node->isLeaf() && node->data.size() >= 8) {
            subdivide(node);

            // Redistribute existing data
            std::vector<T> oldData = std::move(node->data);
            for (const auto& oldItem : oldData) {
                // Assuming T has a position member
                insertHelper(node, oldItem, oldItem.position, depth);
            }
        }

        // Insert into appropriate child
        int octant = getOctant(node->center, position);
        insertHelper(node->children[octant].get(), item, position, depth + 1);
    }

    void subdivide(Node* node) {
        float newHalfSize = node->halfSize * 0.5f;

        for (int i = 0; i < 8; ++i) {
            node->children[i] = std::make_unique<Node>();
            node->children[i]->halfSize = newHalfSize;

            glm::vec3 offset;
            offset.x = ((i & 1) ? 1 : -1) * newHalfSize;
            offset.y = ((i & 2) ? 1 : -1) * newHalfSize;
            offset.z = ((i & 4) ? 1 : -1) * newHalfSize;

            node->children[i]->center = node->center + offset;
        }
    }

    int getOctant(const glm::vec3& center, const glm::vec3& position) const {
        int octant = 0;
        if (position.x > center.x) octant |= 1;
        if (position.y > center.y) octant |= 2;
        if (position.z > center.z) octant |= 4;
        return octant;
    }

    void queryHelper(const Node* node, const glm::vec3& min,
                     const glm::vec3& max, std::vector<T>& results) const {
        if (!node) return;

        // Check if query box intersects with node
        glm::vec3 nodeMin = node->center - glm::vec3(node->halfSize);
        glm::vec3 nodeMax = node->center + glm::vec3(node->halfSize);

        if (min.x > nodeMax.x || max.x < nodeMin.x || min.y > nodeMax.y ||
            max.y < nodeMin.y || min.z > nodeMax.z || max.z < nodeMin.z) {
            return;  // No intersection
        }

        // Add all data in this node that falls within query bounds
        for (const auto& item : node->data) {
            if (item.position.x >= min.x && item.position.x <= max.x &&
                item.position.y >= min.y && item.position.y <= max.y &&
                item.position.z >= min.z && item.position.z <= max.z) {
                results.push_back(item);
            }
        }

        // Recursively query children
        if (!node->isLeaf()) {
            for (const auto& child : node->children) {
                queryHelper(child.get(), min, max, results);
            }
        }
    }
};
