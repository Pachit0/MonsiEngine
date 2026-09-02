#include "MonsiPch.h"
#include "PlatformWindows.h"

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include "Input.h"
#include "MonsiKeyCodes.h"

#include <glad/glad.h>

namespace Monsi {
	static void GLFWErrorCallback(int error_code, const char* description) {
		ENGINE_LOG_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	static uint32_t s_GLFWInitializedCount = 0;

	Window* Window::Create(const WindowInfo& info) {
		return new PlatformWindows(info);
	}

	PlatformWindows::PlatformWindows(const WindowInfo& info) {
		Init(info);
	}

	PlatformWindows::~PlatformWindows() {
		Shutdown();
	}

	void PlatformWindows::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void PlatformWindows::Init(const WindowInfo& info) {
		ENGINE_PROFILER_FUNCTION();

		m_Data.Title = info.Title;
		m_Data.Width = info.Width;
		m_Data.Height = info.Height;

		if (s_GLFWInitializedCount == 0) {
			int success = glfwInit();
			ENGINE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow(info.Width, info.Height, m_Data.Title.c_str(), nullptr, nullptr);
		s_GLFWInitializedCount++;

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
		ENGINE_PROFILER_FUNCTION();

		if (m_Window) {
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}

		if (m_Context) {
			delete m_Context;
			m_Context = nullptr;
		}

		s_GLFWInitializedCount--;
		if (s_GLFWInitializedCount == 0) {
			glfwTerminate();
		}
	}

	void PlatformWindows::OnUpdate() {
		ENGINE_PROFILER_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffer();
	}

	void PlatformWindows::SetVSync(bool enabled) {
		ENGINE_PROFILER_FUNCTION();

		if (enabled) {
			glfwSwapInterval(1);
			ENGINE_LOG_TRACE("VSync {}", enabled);
		}
		else {
			glfwSwapInterval(0);
			ENGINE_LOG_TRACE("VSync {}", enabled);
		}

		m_Data.VSync = enabled;
	}

	bool PlatformWindows::IsVSync() const {
		return m_Data.VSync;
	}

	int PlatformWindows::IsCursorOn() const
	{
		return m_Context->GetCursorStatus();
	}

}