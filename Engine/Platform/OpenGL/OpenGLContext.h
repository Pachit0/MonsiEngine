#pragma once
#include "Renderer/RenderContext.h"

struct GLFWwindow;

namespace Monsi {

	class OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffer() override;
		void EnableCursor() override;
		void DisableCursor() override;

		int GetCursorStatus() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}