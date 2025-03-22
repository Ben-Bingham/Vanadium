#include <iostream>
#include <fstream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"

#include "Utilities/OpenGl/GLDebug.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/OpenGl/Shaders/ShaderProgram.h"

#include "Utilities/ImGui/ImGuiInstance.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(1600, 1000, "Vanadium", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    ImGuiInstance imGui{ };
    imGui.Init(window);

    while (!glfwWindowShouldClose(window)) {
        imGui.StartNewFrame();

        { ImGui::Begin("Viewport");
        ImGui::Text("Hello World!");
        } ImGui::End();

        imGui.FinishFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imGui.Cleanup();

    glfwTerminate();
}
