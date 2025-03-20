#include "GeneralSettings.h"
#include "imgui.h"
#include "ImGuiUtil.h"

#include "Settings/App.h"

#include "Utility/events/Events.h"

namespace Rutile {
    void GeneralSettings() {
        // Select scene
        RadioButtons(
            "Select Scene", 
            {
                "Hello Triangle",
                "Original Scene",
                "Shadow map Testing Scene",
                "Omnidirectional Shadow map Testing Scene",
                "Double Point Light Test Scene",
                "All Spheres",
                "Spheres on Spheres",
                "Hollow Glass Sphere",
                "Ray Tracing In One Weekend",
                "Cornell Box",
                "Backpack",
                "Cornell Box 2.0",
                "8K Triangle Dragon",
                "80K Triangle Dragon",
                "800K Triangle Dragon",
                "Sports Car Front 3/4",
                "Minecraft World"
            }, 
            (int*)&App::sceneType, 
            [] {
                App::scene = SceneManager::GetScene(App::sceneType);

                for (auto object : App::scene.objects) {
                    App::scene.transformBank[object.transform].CalculateMatrix();
                }

                App::renderer->LoadScene();
            }
        );
    }
}