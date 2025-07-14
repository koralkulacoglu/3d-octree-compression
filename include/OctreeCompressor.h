#pragma once
#include "IModelCompressor.h"

class OctreeCompressor : public IModelCompressor {
   public:
    struct Settings {
        int maxDepth;
        int minPointsPerNode;
        float minNodeSize;

        Settings() : maxDepth(8), minPointsPerNode(10), minNodeSize(0.01f) {}
    };

    explicit OctreeCompressor(const Settings& settings);
    OctreeCompressor();  // Default constructor

    std::unique_ptr<CompressedModel> compress(const Model& model) override;

   private:
    Settings settings;
};
