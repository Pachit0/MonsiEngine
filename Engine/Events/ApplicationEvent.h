#pragma once

#include "Core.h"
#include "Events.h"
#include "EventFormatter.h"

namespace Monsi {

	class ENGINE_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}
		
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: width " << m_Width << ", height " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(CategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(CategoryApplication)
	};

	class ENGINE_API AppEventTick : public Event {
	public:
		AppEventTick(){
		
		}

		EVENT_CLASS_CATEGORY(CategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};

	class ENGINE_API AppEventRender : public Event {
	public:
		AppEventRender(){
		
		}

		EVENT_CLASS_CATEGORY(CategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};

	class ENGINE_API AppEventUpdate : public Event {
	public:
		AppEventUpdate(){
		
		}

		EVENT_CLASS_CATEGORY(CategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

}