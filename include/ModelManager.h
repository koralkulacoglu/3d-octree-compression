#pragma once
#include <memory>
#include <string>

class Model;
class CompressedModel;
class IModelLoader;
class IModelCompressor;

class ModelManager {
   public:
    ModelManager();
    ~ModelManager();

    std::unique_ptr<Model> loadModel(const std::string& filename);
    std::unique_ptr<CompressedModel> compressModel(const Model& model);
    std::unique_ptr<CompressedModel> loadCompressedModel(
        const std::string& filename);

   private:
    std::unique_ptr<IModelLoader> loader;
    std::unique_ptr<IModelCompressor> compressor;
};
