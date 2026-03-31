#pragma once

#include "Core.h"
#include "Events.h"

namespace Monsi {

	struct WindowInfo {
		unsigned int Width, Height;
		std::string Title;
		
		WindowInfo(const std::string& WindowTitle = "Monsi Engine", unsigned int width = 1280, unsigned int height = 720)
			: Width(width), Height(height), Title(WindowTitle) { }
	};

	class ENGINE_API Window {
	public:
		using CallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual	void SetEventCallbackFn(const CallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual int IsCursorOn() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowInfo& info = WindowInfo());
	};
}