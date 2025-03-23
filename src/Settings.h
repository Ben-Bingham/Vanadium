#pragma once

#include <PerlinNoise.hpp>

namespace Vanadium {
    struct Settings {
        bool wireframe{ false };

        float planetRadius{ 10.0f };
        bool enableCurvature{ true };

        struct Noise {
            siv::PerlinNoise::seed_type seed{ 123456u };
            siv::PerlinNoise perlin{ seed };

            int octaves{ 1 };
            float amplitude{ 1.0f };
            float mean{ 0.0f };
            float xMult{ 1.0f };
            float zMult{ 1.0f };
        } noise;
    };
}