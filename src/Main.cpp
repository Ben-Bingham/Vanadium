#include <iostream>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

#include "Utilities/OpenGl/Context.h"
#include "Utilities/OpenGl/Shader.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/ImGui/ImGuiInstance.h"
#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"

int main() {
    Context context{ "Vanadium" };
    
    if (!context.Valid()) {
        std::cout << "Context creation failed" << std::endl;
        return -1;
    }

    ImGuiInstance imGui{ };
    imGui.Init(context.Window());

    Shader mainShader{ "assets\\shaders\\main.vert", "assets\\shaders\\main.frag" };
    mainShader.Bind();
    mainShader.SetVec4("color", glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    std::vector<float> vertices{
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    std::vector<unsigned int> indices{
        1, 2, 3
    };

    VertexAttributeObject vao{ };

    VertexBufferObject vbo{ vertices };
    ElementBufferObject ebo{ indices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(context.Window())) {
        imGui.StartNewFrame();

        { ImGui::Begin("Viewport");
        ImGui::Text("Hello World!");
        } ImGui::End();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model{ 1.0f };
        glm::mat4 view = glm::lookAt(glm::vec3{ 0.0f, 0.0f, -5.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), 16.0f / 10.0f, 0.01f, 100.0f); // TODO aspect

        glm::mat4 mvp = projection * view * model;

        mainShader.Bind();
        mainShader.SetMat4("mvp", mvp);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        imGui.FinishFrame();

        glfwSwapBuffers(context.Window());
        glfwPollEvents();
    }

    imGui.Cleanup();
}
