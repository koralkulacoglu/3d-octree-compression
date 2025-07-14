#pragma once
#include <memory>

#include "Octree.h"
#include "VertexData.h"

class Model;

class CompressedModel {
   public:
    using VertexOctree = Octree<VertexData>;

    CompressedModel(std::unique_ptr<VertexOctree> octree,
                    const glm::vec3& minBounds, const glm::vec3& maxBounds);

    std::unique_ptr<Model> decompress() const;
    size_t getCompressedSize() const;
    size_t getVertexCount() const;

   private:
    std::unique_ptr<VertexOctree> octree;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
};
