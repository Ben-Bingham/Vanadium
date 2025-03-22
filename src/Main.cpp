#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include <PerlinNoise.hpp>

#include "imgui.h"

#include "Utilities/OpenGl/Context.h"
#include "Utilities/OpenGl/Shader.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/ImGui/ImGuiInstance.h"
#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"
#include "Utilities/Camera.h"
#include <glm/gtc/type_ptr.hpp>


void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseMovementCallback(GLFWwindow* window, double x, double y);

Camera cam{ };
bool mouseDown{ false };

std::shared_ptr<Window> window{ };

struct Settings {
    bool wireframe{ false };
} settings;

struct DirectionalLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
} dirLight;

struct Phong {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;
} phong;

int main() {
    window = std::make_shared<Window>(glm::ivec2{ 1600, 1000 }, "Vanadium");

    glfwSetFramebufferSizeCallback(window->handle, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window->handle, MouseMovementCallback);

    Context context{ *window };
   
    ImGuiInstance imGui{ };
    imGui.Init(window->handle);

    Shader mainShader{ "assets\\shaders\\main.vert", "assets\\shaders\\main.frag" };
    mainShader.Bind();
    mainShader.SetVec4("color", glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    std::vector<float> vertices{
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
                                                      
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
                                                      
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                                                      
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                                                      
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
                                                      
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f
    };

    std::vector<unsigned int> indices{
        2,  1,  0,
        0,  3,  2,

        4,  5,  6,
        6,  7,  4,

        8,  9, 10,
        10, 11,  8,

        14, 13, 12,
        12, 15, 14,

        16, 17, 18,
        18, 19, 16,

        22, 21, 20,
        20, 23, 22,
    };

    VertexAttributeObject vao{ };

    VertexBufferObject vbo{ vertices };
    ElementBufferObject ebo{ indices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    dirLight.direction = glm::vec3(-0.2f, 1.0f, 0.2f);
    dirLight.ambient = glm::vec3(0.7f);
    dirLight.diffuse = glm::vec3(0.4f);
    dirLight.specular = glm::vec3(0.2f);

    phong.ambient = glm::vec3(0.5f, 0.4f, 0.7f);
    phong.diffuse = phong.ambient * 0.8f;
    phong.specular = phong.ambient * 0.3f;
    phong.shininess = 32.0;

    int n = 8;
    std::vector<std::vector<std::vector<bool>>> grid{ };

    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    float lastFrame{ 0.0f };
    float cleanGridTime{ 0.0f };
    while (!glfwWindowShouldClose(window->handle)) {
        float currentFrame = (float)glfwGetTime();

        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        imGui.StartNewFrame();
        
        // Show GUI
        { ImGui::Begin("Settings");
        ImGui::Text("Frame Time: %fms", dt * 1000.0f);
        ImGui::Text("Time to Clean Grid: %fms", cleanGridTime * 1000.0f);

        if (ImGui::SliderInt("Grid Size", &n, 1, 64) || grid.empty()) {
            grid.clear();

            grid.resize(n);
            for (auto& r : grid) {
                r.resize(n);
                for (auto& c : r) {
                    c.resize(n);
                }
            }

            for (int x = 0; x < n; ++x) {
                for (int z = 0; z < n; ++z) {
                    double noise = perlin.octave2D_01((double)x, (double)z, 1);
                    noise /= 4.0;
                    noise += 0.5;
                    noise *= (double)n;

                    for (int y = 0; y < n; ++y) {
                        if (y >= noise) {
                            break;
                        }

                        grid[x][y][z] = true;
                    }
                }
            }
        }

        ImGui::Checkbox("Wireframe", &settings.wireframe);

        ImGui::Text("Directional Light:");
        ImGui::SliderFloat3("Direction##dirLight", glm::value_ptr(dirLight.direction), -1.0f, 1.0f);
        ImGui::SliderFloat3("Ambient##dirLight", glm::value_ptr(dirLight.ambient), 0.0f, 1.0f);
        ImGui::SliderFloat3("Diffuse##dirLight", glm::value_ptr(dirLight.diffuse), 0.0f, 1.0f);
        ImGui::SliderFloat3("Specular##dirLight", glm::value_ptr(dirLight.specular), 0.0f, 1.0f);

        ImGui::Text("Block material:");
        ImGui::SliderFloat3("Ambient##phong", glm::value_ptr(phong.ambient), 0.0f, 1.0f);
        ImGui::SliderFloat3("Diffuse##phong", glm::value_ptr(phong.diffuse), 0.0f, 1.0f);
        ImGui::SliderFloat3("Specular##phong", glm::value_ptr(phong.specular), 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess##phong", &phong.shininess, 0.0f, 4096.0f);

        } ImGui::End();

        // Settings
        if (settings.wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Basic interaction
        if (glfwGetKey(window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window->handle, true);
        }

        if (glfwGetMouseButton(window->handle, 0) == GLFW_PRESS) {
            mouseDown = true;
        }
        else {
            mouseDown = false;
        }

        // Camera Movement
        if (glfwGetKey(window->handle, GLFW_KEY_W) == GLFW_PRESS) {
            cam.position += cam.forward * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_S) == GLFW_PRESS) {
            cam.position -= cam.forward * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_A) == GLFW_PRESS) {
            cam.position -= cam.right * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_D) == GLFW_PRESS) {
            cam.position += cam.right * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cam.position += cam.up * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cam.position -= cam.up * cam.movementSpeed * dt;
        }

        // Clean Grid
        float cleanGridStart = glfwGetTime();
        auto cleanGrid = grid;

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (!cleanGrid[x][y][z]) continue;

                    bool px = false, py = false, pz = false, nx = false, ny = false, nz = false;

                    if (x + 1 < n) px = grid[x + 1][y][z];
                    if (y + 1 < n) py = grid[x][y + 1][z];
                    if (z + 1 < n) pz = grid[x][y][z + 1];

                    if (x - 1 >= 0) nx = grid[x - 1][y][z];
                    if (y - 1 >= 0) ny = grid[x][y - 1][z];
                    if (z - 1 >= 0) nz = grid[x][y][z - 1];

                    if (px && py && pz && nx && ny && nz) {
                        cleanGrid[x][y][z] = false;
                    }
                }
            }
        }

        cleanGridTime = glfwGetTime() - cleanGridStart;

        // Prep for rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (!cleanGrid[x][y][z]) continue;

                    glm::mat4 model{ 1.0f };
                    model = glm::translate(model, glm::vec3{ (float)x, (float)y, (float)z });

                    glm::mat4 view = cam.ViewMatrix();
                    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)window->size.x / (float)window->size.y, 0.01f, 100.0f);

                    glm::mat4 mvp = projection * view * model;

                    mainShader.SetMat4("mvp", mvp);
                    mainShader.SetMat4("model", model);
                    mainShader.SetVec3("cameraPosition", cam.position);

                    mainShader.SetVec3("dirLight.direction", dirLight.direction);
                    mainShader.SetVec3("dirLight.ambient", dirLight.ambient);
                    mainShader.SetVec3("dirLight.diffuse", dirLight.diffuse);
                    mainShader.SetVec3("dirLight.specular", dirLight.specular);

                    mainShader.SetFloat("phong.shininess", phong.shininess);
                    mainShader.SetVec3("phong.ambient", phong.ambient);
                    mainShader.SetVec3("phong.diffuse", phong.diffuse);
                    mainShader.SetVec3("phong.specular", phong.specular);

                    // Render
                    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, nullptr);
                }
            }
        }

        // Finish the GUI frame
        imGui.FinishFrame();

        // Finish the frame
        glfwSwapBuffers(window->handle);
        glfwPollEvents();
    }

    imGui.Cleanup();
}

void FramebufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
    
    window->size.x = width;
    window->size.y = height;
}

void MouseMovementCallback(GLFWwindow* window, double x, double y) {
    glm::vec2 pos{ (float)x, (float)y };

    if (!mouseDown) {
        cam.lastMousePos = pos;

        return;
    }


    if (cam.lastMousePos.x == std::numeric_limits<float>::max()) {
        cam.lastMousePos = pos;
    }

    glm::vec2 posOffset{ pos.x - cam.lastMousePos.x, cam.lastMousePos.y - pos.y };

    cam.lastMousePos = pos;

    posOffset *= cam.lookSensitivity;

    cam.yaw += posOffset.x;
    cam.pitch += posOffset.y;

    cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);

    cam.forward.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.forward.y = sin(glm::radians(cam.pitch));
    cam.forward.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.forward = glm::normalize(cam.forward);

    cam.right = glm::normalize(glm::cross(cam.forward, cam.up));
}
