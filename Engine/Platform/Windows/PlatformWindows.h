#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>

namespace Monsi {

	class PlatformWindows : public Window {
	public:
		PlatformWindows(const WindowInfo& info);
		virtual ~PlatformWindows();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallbackFn(const CallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowInfo& info);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct PlatformWindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			CallbackFn EventCallback;
		};

		PlatformWindowData m_Data;

		void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	};
}