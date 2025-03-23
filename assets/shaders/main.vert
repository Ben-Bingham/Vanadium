#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;

uniform mat4 mvp;
uniform mat4 model;

out vec3 normal;
out vec3 fragPosition;
out vec2 uv;

uniform int blockId;
uniform int atlasWidth;
uniform int atlasHeight;

void main() {
	gl_Position = mvp * vec4(inPos.x, inPos.y, inPos.z, 1.0);

	normal = inNormal;
	fragPosition = vec3(model * vec4(inPos, 1.0));

	uv = inUv * 0.5;

    uv.x += (0.5 * int(mod(blockId - 1, 2)));
    uv.y += (0.5 * int(floor((blockId - 1) / 2)));
}
