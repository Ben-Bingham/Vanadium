#include "ShaderProgram.h"
#include <algorithm>
#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Utilities/ReadFile.h"

ShaderProgram::ShaderProgram() {
    m_ShaderProgramHandle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_ShaderProgramHandle);
}

void ShaderProgram::Bind() {
    glUseProgram(m_ShaderProgramHandle);
}

unsigned ShaderProgram::Get() {
    return m_ShaderProgramHandle;
}

void ShaderProgram::AddShader(const std::string& shaderPath, ShaderType shaderType) {
    std::string shaderSourceStr = ReadFile(shaderPath);

    AddDefines(shaderSourceStr);

    const char* shaderSource = shaderSourceStr.c_str();

    unsigned int shader = glCreateShader((int)shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR: Vertex shader: " << shaderPath << ", failed to compile:" << std::endl;
        std::cout << infoLog << std::endl;
    }

    glAttachShader(m_ShaderProgramHandle, shader);

    m_Shaders.push_back(shader);
}

void ShaderProgram::Link() {
    glLinkProgram(m_ShaderProgramHandle);

    int success;
    char infoLog[512];

    glGetProgramiv(m_ShaderProgramHandle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ShaderProgramHandle, 512, nullptr, infoLog);
        std::cout << "ERROR: Shader program failed to link:" << std::endl;
        std::cout << infoLog << std::endl;
    }

    for (auto& shader : m_Shaders) {
        glDeleteShader(shader);
    }

    m_Shaders.clear();
}

void ShaderProgram::AddDefine(const std::string& name, const std::string& value) {
    if (!m_Shaders.empty()) {
        std::cout << "ERROR: Adding a define to a shader program that already has shaders. Any previously added shaders will not contain the new defines." << std::endl;
    }

    m_Defines.push_back(std::make_pair(name, value));
}

void ShaderProgram::SetFloat(const std::string& name, const float& value) {
    glUniform1f(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), value);
}

void ShaderProgram::SetInt(const std::string& name, const int& value) {
    glUniform1i(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), value);
}

void ShaderProgram::SetVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetVec2(const std::string& name, const glm::ivec2& value) {
    glUniform2iv(glGetUniformLocation(m_ShaderProgramHandle, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetBool(const std::string& name, const bool& value) {
    int realValue = 0;
    if (value) {
        realValue = 1;
    }

    SetInt(name, realValue);
}

void ShaderProgram::AddDefines(std::string& shaderSource) {
    for (const auto& define : m_Defines) {
        std::string oldDefineString = "#define " + define.first;
        std::string newDefineString = "#define " + define.first + " " + define.second;

        size_t defineLocation = shaderSource.find(oldDefineString);
        size_t nextNewLine = std::string{ shaderSource.substr(defineLocation) }.find('\n') + defineLocation;

        if (defineLocation == std::string::npos || nextNewLine == std::string::npos) {
            continue;
        }

        shaderSource = shaderSource.substr(0, defineLocation) + newDefineString + shaderSource.substr(nextNewLine);
    }
}
