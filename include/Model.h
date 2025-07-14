#pragma once
#include <glm/glm.hpp>
#include <vector>

class Model {
   public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    void calculateBounds();
    bool isValid() const;
};
