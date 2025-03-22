#pragma once

#include <string>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

class Context {
public:
	Context(const std::string& windowName = "Window");
	~Context();

	bool Valid();

	GLFWwindow* Window();

private:
	GLFWwindow* m_Window;
};