#version 330 core

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} dirLight;

struct Phong {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} phong;

out vec4 outFragColor;

uniform vec4 color;

uniform vec3 cameraPosition;

in vec3 normal;
in vec3 fragPosition;

vec3 DirectionalLightAddition(DirectionalLight light, Phong phong, vec3 normal, vec3 viewDir);

void main() {
    dirLight.direction = vec3(1.0, 1.0, 1.0);
    dirLight.ambient   = vec3(0.4);
    dirLight.diffuse   = vec3(0.1);
    dirLight.specular  = vec3(0.001);

    phong.ambient   = vec3(0.3, 0.6, 0.2);
    phong.diffuse   = phong.ambient * 0.4;
	phong.specular  = phong.ambient * 0.1;
	phong.shininess = 32.0;

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPosition - fragPosition);

    outFragColor = vec4(DirectionalLightAddition(dirLight, phong, norm, viewDir), 1.0);
}

vec3 DirectionalLightAddition(DirectionalLight light, Phong phong, vec3 normal, vec3 viewDir) {
    vec3 lightDirection = normalize(light.direction);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), phong.shininess);

    vec3 ambient = light.ambient * phong.ambient;
    vec3 diffuse = light.diffuse * diff * phong.diffuse;
    vec3 specular = light.specular * spec * phong.specular;

    return ambient + diffuse + specular;
}
