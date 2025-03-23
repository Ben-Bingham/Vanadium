#pragma once

#include "Settings.h"
#include "Phong.h"
#include "DirectionalLight.h"

#include "Utilities/OpenGl/Shader.h"

namespace Vanadium {
	void GUI(
		Settings& settings, 
		bool& remakeGrid, 
		int& n, 
		Shader& mainShader,
		Phong& phong,
		DirectionalLight& dirLight,
		float dt
	);
}