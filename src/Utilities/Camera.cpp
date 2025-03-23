#include "camera.h"

#include <glm/ext/matrix_transform.hpp>

Camera::Camera(float movementSpeed, float lookSensitivity)
    : movementSpeed(movementSpeed), lookSensitivity(lookSensitivity) { 

    CalculateVectors();
}

void Camera::CalculateVectors() {
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    right = glm::normalize(glm::cross(forward, up));
}

glm::mat4 Camera::ViewMatrix() {
    return glm::lookAt(position, position + forward, up);
}
