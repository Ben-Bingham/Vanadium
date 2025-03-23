#pragma once

#include <limits>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    Camera(float movementSpeed = 4.0f, float lookSensitivity = 0.4f);

    void CalculateVectors();

    glm::mat4 ViewMatrix();

    glm::vec3 position{ 20.0f, 20.0f, 20.0f };

    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 forward{ 0.0f, 0.0f, 1.0f };
    glm::vec3 right{ 1.0f, 0.0f, 0.0f };

    float yaw{ -135.0f };
    float pitch{ -35.0f };

    float movementSpeed;
    float lookSensitivity;

    glm::vec2 lastMousePos{ std::numeric_limits<float>::max() };
};
