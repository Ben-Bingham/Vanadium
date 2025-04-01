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

#include "Chunks/Chunk.h"
#include "Chunks/GenerateGrid.h"
#include "Chunks/CleanGrid.h"
#include "Chunks/GenerateGeometry.h"
#include "Chunks/CleanGrid.h"
#include "Settings.h"
#include "GUI.h"
#include "JobSystem.h"

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
    TextureParameters p {
        TextureFormat::RGB,
        TextureStorageType::UNSIGNED_BYTE,
        TextureWrapMode::REPEAT,
        TextureFilteringMode::NEAREST
    };

    Texture2D_Array atlas{ {
        "assets\\blocks\\grass.png",
        "assets\\blocks\\dirt.png",
        "assets\\blocks\\stone.png"
    }, p, false };

    mainShader.Bind();
    mainShader.SetInt("newAtlas", 0);

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

    // Initial Chunk Creation
    int n = 4;

    Vanadium::JobSystem jobSystem{ 8 };

    int chunkDistance = 8; // Number of chunks in each cardinal direction (Including up and down) past the players current chunk

    Vanadium::ChunkPosition cameraChunkPosition{ glm::floor(cam.position / (float)n) };
    Vanadium::ChunkPosition lastFrameCameraChunkPosition = cameraChunkPosition;

    std::vector<Vanadium::Job> jobs;
    for (int x = cameraChunkPosition.x - chunkDistance; x < cameraChunkPosition.x + chunkDistance + 1; ++x) {
        for (int y = cameraChunkPosition.y - chunkDistance; y < cameraChunkPosition.y + chunkDistance + 1; ++y) {
            for (int z = cameraChunkPosition.z - chunkDistance; z < cameraChunkPosition.z + chunkDistance + 1; ++z) {
                Vanadium::ChunkPosition cPos = { x, y, z };

                int manhattenDistance = (int)std::abs(cPos.x - cameraChunkPosition.x) + (int)std::abs(cPos.y - cameraChunkPosition.y) + (int)std::abs(cPos.z - cameraChunkPosition.z);

                Vanadium::ChunkPosition furthestChunk{ cameraChunkPosition + chunkDistance };
                int maxManhattenDistance = (int)std::abs(furthestChunk.x - cameraChunkPosition.x) + (int)std::abs(furthestChunk.y - cameraChunkPosition.y) + (int)std::abs(furthestChunk.z - cameraChunkPosition.z);

                size_t priority = (size_t)(maxManhattenDistance - manhattenDistance);

                jobs.push_back(Vanadium::Job{ cPos, settings, n, priority });
            }
        }
    }

    jobSystem.AddJobs(jobs);

    std::vector<Vanadium::Chunk> chunks{ };

    mainShader.Bind();
    mainShader.SetFloat("radius", settings.planetRadius);
    mainShader.SetInt("enableCurvature", settings.enableCurvature);

    glm::vec3 lastFrameCamPos = cam.position;

    float lastFrame{ 0.0f };
    while (!glfwWindowShouldClose(window->handle)) {
        float currentFrame = (float)glfwGetTime();

        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Fetching new chunks
        std::vector<Vanadium::Chunk> newChunks = jobSystem.GetResults();

        for (auto& chunk : newChunks) {
            auto found = std::find_if(chunks.begin(), chunks.end(), [&](const auto& c) { return chunk.position == c.position; });
            if (found != chunks.end()) {
                chunks.erase(found);
            }

            chunk.vao = std::make_unique<VertexAttributeObject>();
            chunk.vbo = std::make_unique<VertexBufferObject>();
            chunk.ebo = std::make_unique<ElementBufferObject>();

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)(8 * sizeof(float)));
            glEnableVertexAttribArray(3);

            chunk.vao->Bind();

            chunk.vbo->UpdateData(Vanadium::VerticesAsFloatVector(chunk.geometry.vertices));
            chunk.ebo->UpdateData(chunk.geometry.indices);

            chunks.push_back(std::move(chunk));
        }

        imGui.StartNewFrame();
        
        bool rg = false;
        Vanadium::GUI(settings, rg, n, mainShader, phong, dirLight, dt);

        // Remaking all chunks due to updated settings
        if (rg) {
            jobSystem.ClearJobs();

            std::vector<Vanadium::Job> jobs;
            for (int x = cameraChunkPosition.x - chunkDistance; x < cameraChunkPosition.x + chunkDistance + 1; ++x) {
                for (int y = cameraChunkPosition.y - chunkDistance; y < cameraChunkPosition.y + chunkDistance + 1; ++y) {
                    for (int z = cameraChunkPosition.z - chunkDistance; z < cameraChunkPosition.z + chunkDistance + 1; ++z) {
                        Vanadium::ChunkPosition cPos = { x, y, z };

                        int manhattenDistance = (int)std::abs(cPos.x - cameraChunkPosition.x) + (int)std::abs(cPos.y - cameraChunkPosition.y) + (int)std::abs(cPos.z - cameraChunkPosition.z);

                        Vanadium::ChunkPosition furthestChunk{ cameraChunkPosition + chunkDistance };
                        int maxManhattenDistance = (int)std::abs(furthestChunk.x - cameraChunkPosition.x) + (int)std::abs(furthestChunk.y - cameraChunkPosition.y) + (int)std::abs(furthestChunk.z - cameraChunkPosition.z);

                        size_t priority = (size_t)(maxManhattenDistance - manhattenDistance);

                        jobs.push_back(Vanadium::Job{ cPos, settings, n, priority });
                    }
                }
            }

            if (!jobs.empty()) {
                jobSystem.AddJobs(jobs);
            }

            chunks.clear();

            chunks = jobSystem.GetResults();

            for (auto& chunk : chunks) {
                chunk.vao = std::make_unique<VertexAttributeObject>();
                chunk.vbo = std::make_unique<VertexBufferObject>();
                chunk.ebo = std::make_unique<ElementBufferObject>();

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)(6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vanadium::Vertex), (void*)(8 * sizeof(float)));
                glEnableVertexAttribArray(3);

                chunk.vao->Bind();
                chunk.vbo->UpdateData(Vanadium::VerticesAsFloatVector(chunk.geometry.vertices));
                chunk.ebo->UpdateData(chunk.geometry.indices);
            }
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

        // Chunk Boundry Crossing
        cameraChunkPosition = Vanadium::ChunkPosition{ glm::floor(cam.position / (float)n) };

        //Vanadium::ChunkPosition delta = chunkCamPosition - lastChunkCamPosition; // TODO

        if (cameraChunkPosition != lastFrameCameraChunkPosition) {
            double st1 = glfwGetTime();
            std::vector<Vanadium::ChunkPosition> allowedChunkPositions{ };

            std::vector<Vanadium::Job> jobs;
            for (int x = cameraChunkPosition.x - chunkDistance; x < cameraChunkPosition.x + chunkDistance + 1; ++x) {
                for (int y = cameraChunkPosition.y - chunkDistance; y < cameraChunkPosition.y + chunkDistance + 1; ++y) {
                    for (int z = cameraChunkPosition.z - chunkDistance; z < cameraChunkPosition.z + chunkDistance + 1; ++z) {
                        allowedChunkPositions.push_back(Vanadium::ChunkPosition{ x, y, z });

                        if (std::find_if(chunks.begin(), chunks.end(), [&](const auto& c) { return Vanadium::ChunkPosition{ x, y, z } == c.position; }) != chunks.end()) {
                            continue;
                        }

                        Vanadium::ChunkPosition cPos = { x, y, z };

                        int manhattenDistance = (int)std::abs(cPos.x - cameraChunkPosition.x) + (int)std::abs(cPos.y - cameraChunkPosition.y) + (int)std::abs(cPos.z - cameraChunkPosition.z);

                        Vanadium::ChunkPosition furthestChunk{ cameraChunkPosition + chunkDistance };
                        int maxManhattenDistance = (int)std::abs(furthestChunk.x - cameraChunkPosition.x) + (int)std::abs(furthestChunk.y - cameraChunkPosition.y) + (int)std::abs(furthestChunk.z - cameraChunkPosition.z);

                        size_t priority = (size_t)(maxManhattenDistance - manhattenDistance);

                        jobs.push_back(Vanadium::Job{ cPos, settings, n, priority });
                    }
                }
            }

            std::cout << "1: " << glfwGetTime() - st1 << std::endl;
            double st2 = glfwGetTime();

            if (!jobs.empty()) {
                jobSystem.AddJobs(jobs);
            }

            std::cout << "2: " << glfwGetTime() - st2 << std::endl;
            double st3 = glfwGetTime();

            std::erase_if(chunks, [&](const Vanadium::Chunk& c){
                if (std::find_if(allowedChunkPositions.begin(), allowedChunkPositions.end(), [&](const auto& ch) { return c.position == ch; }) != allowedChunkPositions.end()) {
                    return false;
                }

                return true;
            });

            std::cout << "3: " << glfwGetTime() - st3 << std::endl;
        }

        lastFrameCamPos = cam.position;
        lastFrameCameraChunkPosition = cameraChunkPosition;

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

        glActiveTexture(GL_TEXTURE0);
        atlas.Bind();

        // Render
        size_t i = 0;
        for (auto& chunk : chunks) {
            if (chunk.geometry.indices.size() == 0) continue; // TODO make sure these are not even added
            chunk.vao->Bind();

            glDrawElements(GL_TRIANGLES, (unsigned int)chunk.geometry.indices.size(), GL_UNSIGNED_INT, nullptr);
            ++i;
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

    cam.CalculateVectors();
}
