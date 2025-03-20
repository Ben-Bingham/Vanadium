#include "VoxelRayTracing.h"
#include "imgui.h"
#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <stack>

#include "GUI/ImGuiUtil.h"

#include "Settings/App.h"

#include "Utility/Random.h"
#include "Utility/events/Events.h"
#include "Utility/OpenGl/GLDebug.h"
#include "Utility/RayTracing/AABB.h"
#include "Utility/RayTracing/AABBFactory.h"

namespace Rutile {
    GLFWwindow* VoxelRayTracing::Init() {
        m_RendererLoadTime = std::chrono::steady_clock::now();

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        GLFWwindow* window = glfwCreateWindow(App::screenWidth, App::screenHeight, App::name.c_str(), nullptr, nullptr);
        glfwShowWindow(window);

        if (!window) {
            std::cout << "ERROR: Failed to create window." << std::endl;
        }

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK) {
            std::cout << "ERROR: Failed to initialize GLEW." << std::endl;
        }

        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        m_MaterialBank = std::make_unique<SSBO<LocalMaterial>>(0);
        m_VoxelSSBO = std::make_unique<SSBO<Voxel>>(5);

        std::vector<float> vertices = {
            // Positions
            -1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3
        };

        m_VoxelRayTracingShader = std::make_unique<Shader>("assets\\shaders\\VoxelRayTracing.vert", "assets\\shaders\\VoxelRayTracing.frag");
        m_RenderingShader = std::make_unique<Shader>("assets\\shaders\\Rendering.vert", "assets\\shaders\\Rendering.frag");

        // Screen Rectangle
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Accumulation Framebuffer
        glGenFramebuffers(1, &m_AccumulationFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_AccumulationFrameBuffer);

        glGenTextures(1, &m_AccumulationTexture);
        glBindTexture(GL_TEXTURE_2D, m_AccumulationTexture);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_AccumulationTexture, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, App::screenWidth, App::screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenRenderbuffers(1, &m_AccumulationRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_AccumulationRBO);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, App::screenWidth, App::screenHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_AccumulationRBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: Accumulation Framebuffer is not complete" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_VoxelRayTracingShader->Bind();

        ResetAccumulatedPixelData();

        return window;
    }

    template<typename T>
    struct _VoxelValue {
        bool GetBool() {
            return (materialIndex & 0x00000001) == 1;
        }

        void SetBool(bool newVal) {
            if (newVal) {
                materialIndex |= 1;
            } else {
                materialIndex &= 0xFFFFFFFE;
            }
        }

        int GetMaterialIndex() {
            return materialIndex >> 1;
        }

        void SetMaterialIndex(T newIndex) {
            bool b = GetBool();
            materialIndex = newIndex << 1;
            SetBool(b);
        }

    private:
        T materialIndex{ 0 };
    };

    using VoxelValue = _VoxelValue<uint8_t>;

    struct Grid {
        Grid(int n)
            : n{ n } {

            grid.resize(n * n * n);
        }

        VoxelValue& Get(int x, int y, int z) {
            return grid[x * n * n + y * n + z];
        }

        void Set(int x, int y, int z, VoxelValue val) {
            grid[x * n * n + y * n + z] = val;
        }

    private:
        int n;

        std::vector<VoxelValue> grid{ };
    };

    void Voxelify(int n, Grid& grid, std::vector<VoxelRayTracing::Voxel>& voxels, glm::vec3 minBound, glm::vec3 maxBound, int currentVoxelIndex = -1, bool first = true) {
        if (first) {
            // Set up the root
            voxels.push_back(VoxelRayTracing::Voxel{ });

            Voxelify(n, grid, voxels, minBound, maxBound, 0, false);

            return;
        }

        int voxelsFound = 0;

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (grid.Get(x, y, z).GetBool()) {
                        ++voxelsFound;
                    }
                }
            }
        }

        VoxelRayTracing::Voxel& currentVoxel = voxels[currentVoxelIndex];
        currentVoxel.minBound = minBound;
        currentVoxel.maxBound = maxBound;

        if (n * n * n == voxelsFound || (n == 1 && grid.Get(0, 0, 0).GetBool())) {
            currentVoxel.hasKids = false;
            currentVoxel.shouldDraw = true;

            currentVoxel.k0 = grid.Get(0, 0, 0).GetMaterialIndex();
        }
        else {
            if (voxelsFound != 0) {
                currentVoxel.hasKids = true;
                currentVoxel.shouldDraw = false;

                int firstOfNextVoxelsIndex = voxels.size();

                // Define children
                std::array<int, 8> voxelCountInChildren{ };
                std::vector<Grid> childrenGrids{ };
                std::array<AABB, 8> childrenBounds{ };

                int hN = n / 2;
                float kidWidth = (maxBound.x - minBound.x) / 2.0f;

                for (int i = 0; i < 8; ++i) {
                    childrenGrids.push_back(Grid{ n / 2 });
                    Grid g{ n / 2 };

                    glm::vec3 minB{ };
                    glm::vec3 maxB{ };

                    glm::ivec3 sourceOffset{ };

                    switch (i) {
                    case 0:
                        minB = minBound;
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ 0 };

                        break;

                    case 1:
                        minB = minBound + glm::vec3{ kidWidth, 0.0f, 0.0f };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ hN, 0, 0 };

                        break;
                    case 2:
                        minB = minBound + glm::vec3{ 0.0f, 0.0f, kidWidth };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ 0, 0, hN };

                        break;
                    case 3:
                        minB = minBound + glm::vec3{ kidWidth, 0.0f, kidWidth };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ hN, 0, hN };

                        break;
                    case 4:
                        minB = minBound + glm::vec3{ 0.0f, kidWidth, 0.0f };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ 0, hN, 0 };

                        break;
                    case 5:
                        minB = minBound + glm::vec3{ kidWidth, kidWidth, 0.0f };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ hN, hN, 0 };

                        break;
                    case 6:
                        minB = minBound + glm::vec3{ 0.0f, kidWidth, kidWidth };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ 0, hN, hN };

                        break;
                    case 7:
                        minB = minBound + glm::vec3{ kidWidth, kidWidth, kidWidth };
                        maxB = minB + kidWidth;

                        sourceOffset = glm::vec3{ hN, hN, hN };

                        break;
                    }

                    int voxelCount = 0;
                    for (int x = 0; x < hN; ++x) {
                        for (int y = 0; y < hN; ++y) {
                            for (int z = 0; z < hN; ++z) {
                                g.Set(x, y, z, grid.Get(x + sourceOffset.x, y + sourceOffset.y, z + sourceOffset.z));
                                if (g.Get(x,y,z).GetBool()) ++voxelCount;
                            }
                        }
                    }

                    voxelCountInChildren[i] = voxelCount;
                    childrenGrids[i] = g;
                    childrenBounds[i].min = minB;
                    childrenBounds[i].max = maxB;
                }

                // All voxels need to be added first in order to insure that they are contiguous in memory
                for (int i = 0; i < 8; ++i) {
                    if (voxelCountInChildren[i] != 0) {
                        voxels.push_back(VoxelRayTracing::Voxel{ });
                    }
                }

                // Set the index of just the first octant
                voxels[currentVoxelIndex].k0 = firstOfNextVoxelsIndex;

                int childrenAdded = 0;
                for (int i = 0; i < 8; ++i) {
                    if (voxelCountInChildren[i] == 0) continue;

                    // Enable the correct bit in the child mask
                    switch (i) {
                        case 0: voxels[currentVoxelIndex].childMask |= 1  ; break;
                        case 1: voxels[currentVoxelIndex].childMask |= 2  ; break;
                        case 2: voxels[currentVoxelIndex].childMask |= 4  ; break;
                        case 3: voxels[currentVoxelIndex].childMask |= 8  ; break;
                        case 4: voxels[currentVoxelIndex].childMask |= 16 ; break;
                        case 5: voxels[currentVoxelIndex].childMask |= 32 ; break;
                        case 6: voxels[currentVoxelIndex].childMask |= 64 ; break;
                        case 7: voxels[currentVoxelIndex].childMask |= 128; break;
                    }

                    Voxelify(n / 2, childrenGrids[i], voxels, childrenBounds[i].min, childrenBounds[i].max, firstOfNextVoxelsIndex + childrenAdded, false);

                    ++childrenAdded;
                }
            }
            else {
                currentVoxel.hasKids = false;
                currentVoxel.shouldDraw = false;
            }
        }
    }

    struct AABBMatId {
        AABB bbox;
        MaterialIndex matId;
        Object obj;
    };

    void VoxelRayTracing::CreateOctree() {
        /*
         *   ---------        ---------
         *   | 0 | 1 |        | 4 | 5 |
         *   ---------> +X    ---------> +X
         *   | 2 | 3 |        | 6 | 7 |
         *   ---------        ---------
         *       V                V
         *       +Z               +Z
         */

        constexpr int n = 256;
        float largestVal = 7.0f;

        glm::vec3 min{ -largestVal };
        glm::vec3 max{ largestVal };

        Grid grid{ n };

        voxels.clear();

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (y < n / 2) {
                        grid.Get(x, y, z).SetBool(true);
                        grid.Get(x, y, z).SetMaterialIndex(1);
                    }
                }
            }
        }

        Voxelify(n, grid, voxels, min, max);

        m_VoxelRayTracingShader->Bind();

        m_VoxelSSBO->SetData(voxels);

        m_VoxelRayTracingShader->SetInt("octreeRootIndex", 0);
    }

    void VoxelRayTracing::Cleanup(GLFWwindow* window) {
        m_VoxelRayTracingShader.reset();
        m_RenderingShader.reset();

        m_VoxelSSBO.reset();

        glfwDestroyWindow(window);
    }

    void VoxelRayTracing::Notify(Event* event) {
        if (EVENT_IS(event, WindowResize)) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_AccumulationFrameBuffer);
            glBindTexture(GL_TEXTURE_2D, m_AccumulationTexture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, App::screenWidth, App::screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_AccumulationTexture, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, m_AccumulationRBO);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, App::screenWidth, App::screenHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_AccumulationRBO);

            ResetAccumulatedPixelData();
        }
        if (EVENT_IS(event, CameraUpdate) ||
            EVENT_IS(event, ObjectTransformUpdate) ||
            EVENT_IS(event, ObjectMaterialUpdate)) {

            ResetAccumulatedPixelData();
        }
        if (EVENT_IS(event, ObjectMaterialUpdate)) {
            CreateAndUploadMaterialBuffer();
        }
    }
    void VoxelRayTracing::Render() {
        m_VoxelRayTracingShader->Bind();

        ++m_FrameCount;

        // Render into accumulation framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_AccumulationFrameBuffer);
        glViewport(0, 0, App::screenWidth, App::screenHeight);

        m_VoxelRayTracingShader->Bind();

        const glm::mat4 cameraProjection = glm::perspective(glm::radians(App::settings.fieldOfView), (float)App::screenWidth / (float)App::screenHeight, App::settings.nearPlane, App::settings.farPlane);
        const glm::mat4 inverseProjection = glm::inverse(cameraProjection);

        const glm::mat4 inverseView = glm::inverse(App::camera.View());

        m_VoxelRayTracingShader->SetFloat("miliTime", (float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_RendererLoadTime).count() / 1000.0f);
        m_VoxelRayTracingShader->SetInt("screenWidth", App::screenWidth);
        m_VoxelRayTracingShader->SetInt("screenHeight", App::screenHeight);

        m_VoxelRayTracingShader->SetMat4("invView", inverseView);
        m_VoxelRayTracingShader->SetMat4("invProjection", inverseProjection);
        m_VoxelRayTracingShader->SetVec3("cameraPosition", App::camera.position);

        m_VoxelRayTracingShader->SetInt("maxBounces", App::settings.maxBounces);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_AccumulationTexture);
        m_VoxelRayTracingShader->SetInt("accumulationBuffer", 0);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Read from accumulation framebuffer, divide by frame count, and render to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, App::screenWidth, App::screenHeight);

        glClearColor(App::settings.backgroundColor.b, App::settings.backgroundColor.g, App::settings.backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_RenderingShader->Bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_AccumulationTexture);
        m_RenderingShader->SetInt("accumulationBuffer", 0);

        m_RenderingShader->SetInt("frameCount", (int)m_FrameCount);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void VoxelRayTracing::LoadScene() {
        CreateOctree();

        CreateAndUploadMaterialBuffer();

        ResetAccumulatedPixelData();
    }

    void VoxelRayTracing::ProvideLocalRendererSettings() {
        static int maxBboxChecks = 100;
        static int maxSphereChecks = 100;
        static int maxTriangleChecks = 100;
        static int maxMeshChecks = 100;

        static int mode = 0;

        RadioButtons("Stats mode", std::vector<std::string>{
            "Bounding Boxes",
                "Spheres",
                "Triangles",
                "Meshes"
        },
            & mode
        );

        int bbox = -1;
        int sphere = -1;
        int tri = -1;
        int mesh = -1;

        switch (mode) {
        case 0:
            if (ImGui::DragInt("Max Bounding Box Checks", &maxBboxChecks, 0.1f, 0, 10000)) {
                ResetAccumulatedPixelData();
            }
            bbox = maxBboxChecks;
            break;

        case 1:
            if (ImGui::DragInt("Max Sphere Checks", &maxSphereChecks, 0.1f, 0, 10000)) {
                ResetAccumulatedPixelData();
            }
            sphere = maxSphereChecks;
            break;

        case 2:
            if (ImGui::DragInt("Max Triangle Checks", &maxTriangleChecks, 0.1f, 0, 10000)) {
                ResetAccumulatedPixelData();
            }
            tri = maxTriangleChecks;
            break;

        case 3:
            if (ImGui::DragInt("Max Mesh Checks", &maxMeshChecks, 0.1f, 0, 10000)) {
                ResetAccumulatedPixelData();
            }
            mesh = maxMeshChecks;
            break;

        default:
            std::cout << "ERROR: Unknown mode" << std::endl;
            break;
        }

        m_VoxelRayTracingShader->Bind();
        m_VoxelRayTracingShader->SetInt("maxBboxChecks", bbox);
        m_VoxelRayTracingShader->SetInt("maxSphereChecks", sphere);
        m_VoxelRayTracingShader->SetInt("maxTriangleChecks", tri);
        m_VoxelRayTracingShader->SetInt("maxMeshChecks", mesh);
    }

    void VoxelRayTracing::ProjectionMatrixUpdate() {
        ResetAccumulatedPixelData();
    }

    void VoxelRayTracing::ResetAccumulatedPixelData() {
        m_FrameCount = 0;

        glBindFramebuffer(GL_FRAMEBUFFER, m_AccumulationFrameBuffer);
        glBindTexture(GL_TEXTURE_2D, m_AccumulationTexture);

        std::vector<glm::vec4> clearData{ };
        clearData.resize((size_t)App::screenWidth * (size_t)App::screenHeight);
        for (auto& val : clearData) {
            val = glm::vec4{ 0.0f };
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, App::screenWidth, App::screenHeight, 0, GL_RGBA, GL_FLOAT, clearData.data());
    }

    void VoxelRayTracing::CreateAndUploadMaterialBuffer() {
        m_VoxelRayTracingShader->Bind();

        std::vector<LocalMaterial> localMats{ };
        for (size_t i = 0; i < App::scene.materialBank.Size(); ++i) {
            LocalMaterial mat{ };
            mat.type = (int)App::scene.materialBank[i].type;
            mat.fuzz = App::scene.materialBank[i].fuzz;
            mat.indexOfRefraction = App::scene.materialBank[i].indexOfRefraction;
            mat.color = glm::vec4{ App::scene.materialBank[i].solid.color, 1.0 };

            localMats.emplace_back(mat);
        }

        m_MaterialBank->SetData(localMats);
    }
}