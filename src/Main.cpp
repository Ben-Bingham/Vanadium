#include <gl/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Settings/App.h"

#include "renderer/renderer.h"
#include "renderer/VoxelRayTracing.h"

#include "GUI/MainGUIWindow.h"

#include "Utility/CameraMovement.h"
#include "Utility/events/Events.h"

using namespace Rutile;

void CreateCurrentRenderer() {
    App::renderer = std::make_unique<VoxelRayTracing>();

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    App::window = App::renderer->Init();

    for (auto object : App::scene.objects) {
        App::scene.transformBank[object.transform].CalculateMatrix();
    }

    App::renderer->LoadScene();
    App::renderer->SignalSettingsUpdate();
    App::renderer->ProjectionMatrixUpdate();

    App::glfw.AttachOntoWindow(App::window);

    App::imGui.Init(App::window);
}

void ShutDownCurrentRenderer() {
    App::imGui.Cleanup();

    App::glfw.DetachFromWindow(App::window);

    App::renderer->Cleanup(App::window);
    App::renderer.reset();
}

int main() {
    App::timingData.startTime = std::chrono::steady_clock::now();

    App::glfw.Init();

    CreateCurrentRenderer();

    // Main loop
    while (!glfwWindowShouldClose(App::window)) {
        auto frameStartTime = std::chrono::steady_clock::now();

        glfwPollEvents();
        App::eventManager.Distribute();

        MoveCamera();

        { // Rendering
            App::imGui.StartNewFrame();

            auto imGuiStartTime = std::chrono::steady_clock::now();
            MainGuiWindow();

            App::timingData.imGuiTime = std::chrono::steady_clock::now() - imGuiStartTime;

            auto renderStartTime = std::chrono::steady_clock::now();
            App::renderer->Render();
            App::timingData.renderTime = std::chrono::steady_clock::now() - renderStartTime;

            App::imGui.FinishFrame();

            glfwSwapBuffers(App::window);
        }

        // Timing the frame
        App::timingData.frameTime = std::chrono::steady_clock::now() - frameStartTime;

        App::timingData.frameTimes.push_back(App::timingData.frameTime);

        if (App::timingData.frameTimes.size() > App::timingData.rollingAverageLength) {
            for (size_t i = 0; i < App::timingData.frameTimes.size() - 1; ++i) {
                App::timingData.frameTimes[i] = App::timingData.frameTimes[i + 1];
            }

            App::timingData.frameTimes.pop_back();
        }

        App::timingData.rollingAverageFrameTime = std::chrono::duration<double>{ 0 };

        for (auto& frameTime : App::timingData.frameTimes) {
            App::timingData.rollingAverageFrameTime += frameTime;
        }

        App::timingData.rollingAverageFrameTime /= (double)App::timingData.frameTimes.size();
    }

    App::imGui.Cleanup();

    App::glfw.DetachFromWindow(App::window);

    App::renderer->Cleanup(App::window);

    App::glfw.Cleanup();
}