#pragma once
#include <glm/glm.hpp>

struct VertexData {
    glm::vec3 position;
    glm::vec3 color;

    VertexData(const glm::vec3& pos, const glm::vec3& col)
        : position(pos), color(col) {}
};
