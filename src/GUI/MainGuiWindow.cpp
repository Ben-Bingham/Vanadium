#include "MainGuiWindow.h"
#include "GeneralSettings.h"

#include "imgui.h"
#include "RayTracing.h"
#include "RenderingSettings.h"

#include "SceneObjects.h"
#include "TimingStatistics.h"

#include "Settings/App.h"

namespace Rutile {
    void MainGuiWindow() {
        //ImGui::ShowDemoWindow();
        //ImPlot::ShowDemoWindow();

        ImGui::Begin("Rutile");
        {
            if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {
                GeneralSettings();
            }

            if (ImGui::CollapsingHeader("Timing Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
                TimingStatistics();
            }

            if (ImGui::CollapsingHeader("Rendering Settings")) {
                RenderingSettings();
            }

            if (ImGui::CollapsingHeader("Ray Tracing Settings")) {
                RayTracingSettings();
            }

            if (ImGui::CollapsingHeader("Local Renderer Settings")) {
                App::renderer->ProvideLocalRendererSettings();
            }

            if (ImGui::CollapsingHeader("Scene Objects")) {
                SceneObjects();
            }
        }
        ImGui::End();
    }
}