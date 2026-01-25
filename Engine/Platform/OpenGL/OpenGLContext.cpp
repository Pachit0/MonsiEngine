#include "MonsiPch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Monsi {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		ENGINE_ASSERT(windowHandle, "WindowHandle didn't initialize properly!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_ASSERT(status, "Failed to initialize Glad!");

		ENGINE_LOG_INFO("OpenGL Info:");
		ENGINE_LOG_INFO("	Vendor: {0} ", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		ENGINE_LOG_INFO("	Renderer: {0} ", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		ENGINE_LOG_INFO("	Version: {0} ", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffer() {
		glfwSwapBuffers(m_WindowHandle);
	}

}