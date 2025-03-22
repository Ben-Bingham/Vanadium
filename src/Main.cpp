#include <iostream>
#include <fstream>

#include "Utilities/OpenGl/Context.h"

#include "imgui.h"

#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/OpenGl/Shaders/ShaderProgram.h"

#include "Utilities/ImGui/ImGuiInstance.h"

int main() {
    Context context{ "Vanadium" };
    
    if (!context.Valid()) {
        std::cout << "Context creation failed" << std::endl;
        return -1;
    }

    ImGuiInstance imGui{ };
    imGui.Init(context.Window());

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
