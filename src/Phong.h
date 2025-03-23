#pragma once

#include <glm/glm.hpp>

namespace Vanadium {
    struct Phong {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float shininess;
    };
}