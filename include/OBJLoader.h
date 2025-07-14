#pragma once
#include "IModelLoader.h"

class OBJLoader : public IModelLoader {
   public:
    std::unique_ptr<Model> load(const std::string& filename) override;
};
