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
            float percentOfBlocksAffected{ 0.25 };
            float xMult{ 1.0f };
            float yMult{ 1.0f };
            float noiseMult{ 1.0f };
            float noiseOffset{ 0.0f };
        } noise;
    };
}