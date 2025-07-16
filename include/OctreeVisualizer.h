#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Octree.h"
#include "VertexData.h"

class OctreeVisualizer {
   public:
    struct BoundingBox {
        glm::vec3 center;
        glm::vec3 halfSize;
        bool hasData;
        int level;
    };

    OctreeVisualizer();

    // Extract bounding boxes from octree up to specified level
    std::vector<BoundingBox> extractBoundingBoxes(
        const Octree<VertexData>* octree, int maxLevel = -1) const;

    // Get wireframe vertices for a bounding box
    std::vector<glm::vec3> getWireframeVertices(const BoundingBox& box) const;

    // Get solid box vertices (for filled rendering)
    std::vector<glm::vec3> getSolidBoxVertices(const BoundingBox& box) const;

   private:
    void extractBoxesRecursive(const typename Octree<VertexData>::Node* node,
                               std::vector<BoundingBox>& boxes,
                               int currentLevel, int maxLevel) const;
};
