#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utilities/OpenGl/Context.h"
#include "Utilities/OpenGl/Shader.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/ImGui/ImGuiInstance.h"
#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"
#include "Utilities/Camera.h"

#include "GridGeneration.h"
#include "Settings.h"
#include "GUI.h"
#include "CleanGrid.h"
#include "GenerateGeometry.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseMovementCallback(GLFWwindow* window, double x, double y);

Camera cam{ };
bool mouseDown{ false };

std::shared_ptr<Window> window{ };

int main() {
    window = std::make_shared<Window>(glm::ivec2{ 1600, 1000 }, "Vanadium");

    glfwSetFramebufferSizeCallback(window->handle, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window->handle, MouseMovementCallback);

    Context context{ *window };
   
    ImGuiInstance imGui{ };
    imGui.Init(window->handle);

    Shader mainShader{ "assets\\shaders\\main.vert", "assets\\shaders\\main.frag" };
    mainShader.Bind();

    Vanadium::Settings settings{ };

    // Texture Atlas
    Texture::Parameters p {
        Texture::Format::RGB,
        Texture::StorageType::UNSIGNED_BYTE,
        Texture::WrapMode::REPEAT,
        Texture::FilteringMode::NEAREST
    };

    Texture atlas{ "assets\\blocks\\atlas.png", p, false };
    mainShader.Bind();
    mainShader.SetInt("textureAtlas", 0);
    mainShader.SetInt("atlasWidth", 2);
    mainShader.SetInt("atlasHeight", 2);

    // Basic OpenGL settings
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Material Settings
    Vanadium::DirectionalLight dirLight{ };
    dirLight.direction = glm::vec3(-0.2f, 1.0f, 0.2f);
    dirLight.ambient = glm::vec3(0.7f);
    dirLight.diffuse = glm::vec3(0.4f);
    dirLight.specular = glm::vec3(0.2f);

    Vanadium::Phong phong{ };
    phong.ambient = glm::vec3(0.5f, 0.4f, 0.7f);
    phong.diffuse = phong.ambient * 0.8f;
    phong.specular = phong.ambient * 0.3f;
    phong.shininess = 32.0;

    int n = 8;
    Vanadium::Grid grid{ };
    bool remakeGrid{ true };

    Vanadium::Geometry geometry{ };

    VertexAttributeObject geoVAO{ };

    VertexBufferObject geoVBO{ Vanadium::VerticesAsFloatVector(geometry.vertices) };
    ElementBufferObject geoEBO{ geometry.indices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    mainShader.Bind();
    mainShader.SetFloat("radius", settings.planetRadius);
    mainShader.SetInt("enableCurvature", settings.enableCurvature);

    float lastFrame{ 0.0f };
    while (!glfwWindowShouldClose(window->handle)) {
        float currentFrame = (float)glfwGetTime();

        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        imGui.StartNewFrame();
        
        Vanadium::GUI(settings, remakeGrid, n, mainShader, phong, dirLight, dt);

        if (remakeGrid) {
            grid = Vanadium::CreateGrid(n, settings);
            grid = Vanadium::CleanGrid(grid, n);

            geometry = Vanadium::GenerateGeometry(grid, n, 2, 2);
            geoVBO.UpdateData(Vanadium::VerticesAsFloatVector(geometry.vertices));
            geoEBO.UpdateData(geometry.indices);

            remakeGrid = false;
        }

        // Basic interaction
        if (glfwGetKey(window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window->handle, true);

        if (glfwGetMouseButton(window->handle, 0) == GLFW_PRESS) mouseDown = true;
        else mouseDown = false;

        // Camera Movement
        if (glfwGetKey(window->handle, GLFW_KEY_W) == GLFW_PRESS) cam.position += cam.forward * cam.movementSpeed * dt;
        if (glfwGetKey(window->handle, GLFW_KEY_S) == GLFW_PRESS) cam.position -= cam.forward * cam.movementSpeed * dt;
        if (glfwGetKey(window->handle, GLFW_KEY_A) == GLFW_PRESS) cam.position -= cam.right * cam.movementSpeed * dt;
        if (glfwGetKey(window->handle, GLFW_KEY_D) == GLFW_PRESS) cam.position += cam.right * cam.movementSpeed * dt;
        if (glfwGetKey(window->handle, GLFW_KEY_SPACE) == GLFW_PRESS) cam.position += cam.up * cam.movementSpeed * dt;
        if (glfwGetKey(window->handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cam.position -= cam.up * cam.movementSpeed * dt;

        // Prep for rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering
        glm::mat4 model{ 1.0f };
        mainShader.SetMat4("model", model);

        glm::mat4 view = cam.ViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)window->size.x / (float)window->size.y, 0.01f, 10000.0f);
        glm::mat4 vp = projection * view;
        mainShader.SetMat4("vp", vp);
        
        mainShader.SetVec3("cameraPosition", cam.position);
        
        mainShader.SetVec3("dirLight.direction", dirLight.direction);
        mainShader.SetVec3("dirLight.ambient", dirLight.ambient);
        mainShader.SetVec3("dirLight.diffuse", dirLight.diffuse);
        mainShader.SetVec3("dirLight.specular", dirLight.specular);
        
        mainShader.SetFloat("phong.shininess", phong.shininess);
        mainShader.SetVec3("phong.ambient", phong.ambient);
        mainShader.SetVec3("phong.diffuse", phong.diffuse);
        mainShader.SetVec3("phong.specular", phong.specular);

        mainShader.SetInt("blockId", 1); // TODO block ID

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas.Get());

        geoVAO.Bind();

        // Render
        glDrawElements(GL_TRIANGLES, (unsigned int)geometry.indices.size(), GL_UNSIGNED_INT, nullptr);

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

    cam.CalculateVectors();
}

/*

// Main Thread:
glm::vec3 lastCamPos = cam.position;

Jobs jobs{ };

int chunkDistance = 6;
int halfchunkDistance = floor(chunkDistance / 2)
for (int x = -halfchunkDistance; x < halfchunkDistance; ++x) {
    for (int y = -halfchunkDistance; y < halfchunkDistance; ++x) {
        jobs.AddJob(glm::ivec2(x, y));
    }
}

while (true) {
    auto freshGeoemtry = jobs.GetFinishedGeometry();
    availableGeometry.insert(freshGeoemtry.begin(), freshGeoemtry.end())

    for (geo : availableGeometry) {
        if (geo.visible) {
            Render(geo);   
        }
    }

    if (lastCamPos != cam.position) {
        if (crossed chunk boundry) {
            // Determine if we crossed an X boundry, or a Z boundry, or at a diagonal, then do a single for loop
            // to figure out which chunks need to be added.
            // Also do this to figure out which chunks to remove.
            for (chunk : newChunks) {
                jobs.AddJob(chunk)
            }

            for (chunk : chunksToRemove) {
                jobs.DelteJob(chunk);
            }
        }
    }
}

*/