#include "GUI.h"

#include "imgui.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Utilities/ImGui/ImGuiInstance.h"


namespace Vanadium {
	void GUI(Settings& settings, bool& remakeGrid, int& n, Shader& mainShader, Phong& phong, DirectionalLight& dirLight, float dt) {        
        { ImGui::Begin("Settings");

            ImGui::Text("Frame Time: %fms", dt * 1000.0f);

            if (ImGui::SliderInt("Grid Size", &n, 1, 64)) {
                remakeGrid = true;
            }

            ImGui::Separator();

            if (ImGui::Checkbox("Wireframe", &settings.wireframe)) {
                if (settings.wireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }

            ImGui::Separator();

            ImGui::Text("Directional Light:");
            ImGui::SliderFloat3("Direction##dirLight", glm::value_ptr(dirLight.direction), -1.0f, 1.0f);
            ImGui::ColorEdit3("Ambient##dirLight", glm::value_ptr(dirLight.ambient));
            ImGui::ColorEdit3("Diffuse##dirLight", glm::value_ptr(dirLight.diffuse));
            ImGui::ColorEdit3("Specular##dirLight", glm::value_ptr(dirLight.specular));

            ImGui::Text("Block material:");
            ImGui::ColorEdit3("Ambient##phong", glm::value_ptr(phong.ambient));
            ImGui::ColorEdit3("Diffuse##phong", glm::value_ptr(phong.diffuse));
            ImGui::ColorEdit3("Specular##phong", glm::value_ptr(phong.specular));
            ImGui::SliderFloat("Shininess##phong", &phong.shininess, 0.0f, 4096.0f);

            ImGui::Separator();

            ImGui::Text("Noise:");
            if (ImGui::SliderInt("Octaves##noise", &settings.noise.octaves, 1, 64)) remakeGrid = true;
            if (ImGui::SliderFloat("Amplitude##noise", &settings.noise.amplitude, 0.001f, 10.0f)) remakeGrid = true;
            if (ImGui::SliderFloat("Mean##noise", &settings.noise.mean, -10.0f, 10.0f)) remakeGrid = true;
            if (ImGui::SliderFloat("X Coordinate Multiplier##noise", &settings.noise.xMult, 0.0f, 10.0f)) remakeGrid = true;
            if (ImGui::SliderFloat("Z Coordinate Multiplier##noise", &settings.noise.zMult, 0.0f, 10.0f)) remakeGrid = true;
            if (ImGui::SliderFloat("X Coordinate Offset##noise", &settings.noise.xOffset, -20.0f, 20.0f)) remakeGrid = true;
            if (ImGui::SliderFloat("Z Coordinate Offset##noise", &settings.noise.zOffset, -20.0f, 20.0f)) remakeGrid = true;

            int s = (int)settings.noise.seed;
            if (ImGui::SliderInt("Seed##noise", &s, 0, (int)std::numeric_limits<int>::max() / 4)) {
                settings.noise.seed = s;
                settings.noise.perlin.reseed(settings.noise.seed);

                remakeGrid = true;
            }

            ImGui::Separator();

            ImGui::Text("Planet Info:");
            if (ImGui::SliderFloat("Radius##planet", &settings.planetRadius, 1.0f, 50.0f)) {
                mainShader.Bind();
                mainShader.SetFloat("radius", settings.planetRadius);
            }

            if (ImGui::Checkbox("Enable Curveature", (bool*)&settings.enableCurvature)) {
                mainShader.Bind();
                mainShader.SetBool("enableCurvature", settings.enableCurvature);
            }

        } ImGui::End();
	}
}