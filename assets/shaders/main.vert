#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;

uniform mat4 vp;
uniform mat4 model;

out vec3 normal;
out vec3 fragPosition;
out vec2 uv;

uniform vec3 cameraPosition;

uniform float radius;
uniform bool enableCurvature;

void main() {
	vec4 pos = model * vec4(inPos.x, inPos.y, inPos.z, 1.0);
	
	if (enableCurvature) {
		float yOffset = -sqrt(radius * radius - (pos.x - cameraPosition.x) * (pos.x - cameraPosition.x) - (pos.z - cameraPosition.z) * (pos.z - cameraPosition.z)) + radius;
		pos.y -= yOffset;
	}

	gl_Position = vp * pos;

	normal = inNormal;
	fragPosition = vec3(model * vec4(inPos, 1.0));

	uv = inUv;
}
