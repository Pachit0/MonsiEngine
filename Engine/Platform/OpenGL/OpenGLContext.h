#pragma once
#include "Renderer/RenderContext.h"

struct GLFWwindow;

namespace Monsi {

	class OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}