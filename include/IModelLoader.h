#pragma once
#include <memory>
#include <string>

class Model;

class IModelLoader {
   public:
    virtual ~IModelLoader() = default;
    virtual std::unique_ptr<Model> load(const std::string& filename) = 0;
};
