#pragma once
#include <memory>

class Model;
class CompressedModel;

class IModelCompressor {
   public:
    virtual ~IModelCompressor() = default;
    virtual std::unique_ptr<CompressedModel> compress(const Model& model) = 0;
};
