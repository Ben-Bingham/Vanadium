#include <iostream>
#include <fstream>

#include "imgui.h"

#include "Utilities/OpenGl/Context.h"
#include "Utilities/OpenGl/Shader.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/ImGui/ImGuiInstance.h"

Shader mainProgram{ "assets\\shaders\\main.vert", "assets\\shaders\\main.frag" };

int main() {
    Context context{ "Vanadium" };
    
    if (!context.Valid()) {
        std::cout << "Context creation failed" << std::endl;
        return -1;
    }

    ImGuiInstance imGui{ };
    imGui.Init(context.Window());

    mainProgram.Bind();

    while (!glfwWindowShouldClose(context.Window())) {
        imGui.StartNewFrame();

        { ImGui::Begin("Viewport");
        ImGui::Text("Hello World!");
        } ImGui::End();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        imGui.FinishFrame();

        glfwSwapBuffers(context.Window());
        glfwPollEvents();
    }

    imGui.Cleanup();
}
