#include "MonsiPch.h"
#include "PlatformWindows.h"

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Monsi {
	static void GLFWErrorCallback(int error_code, const char* description) {
		ENGINE_LOG_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}
	
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowInfo& info) {
		return new PlatformWindows(info);
	}

	PlatformWindows::PlatformWindows(const WindowInfo& info) {
		Init(info);
	}

	PlatformWindows::~PlatformWindows() {

	}

	void PlatformWindows::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void PlatformWindows::Init(const WindowInfo& info) {
		m_Data.Title = info.Title;
		m_Data.Width = info.Width;
		m_Data.Height = info.Height;
		
		ENGINE_LOG_INFO("Creating Window {0} ({1}, {2})", info.Title, info.Width, info.Height);
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			ENGINE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(info.Width, info.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyEventPressed event(key, 0);
					data.EventCallback(event);
					break;
					}
				case GLFW_RELEASE: {
					KeyEventReleased event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyEventPressed event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);

			KeyEventTyped event(keycode);
			data.EventCallback(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);

			MouseEventScrolled event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);

			MouseEventMoved event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			PlatformWindowData& data = *(PlatformWindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
				MouseEventButtonPressed event(button);
				data.EventCallback(event);
				break;
				}
				case GLFW_RELEASE: {
				MouseEventButtonReleased event(button);
				data.EventCallback(event);
				break;
				}
			}
		});

	}

	void PlatformWindows::Shutdown() {

	}

	void PlatformWindows::OnUpdate() {

		glfwPollEvents();
		m_Context->SwapBuffer();
	}
	
	void PlatformWindows::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool PlatformWindows::IsVSync() const {
		return m_Data.VSync;
	}

}