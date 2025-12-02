#pragma once

#include "Events.h"
#include "EventFormatter.h"

namespace Monsi {
	class MouseEventMoved : public Event {
	public:
		MouseEventMoved(float x, float y) : m_MouseX(x), m_MouseY(y) {}

		inline float GetMouseY() const { return m_MouseY; }
		inline float GetMouseX() const { return m_MouseX; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseEventMoved: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(CategoryMouse | CategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseEventButton : public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(CategoryMouse | CategoryInput)
	protected:
		MouseEventButton(int button) : m_Button(button) {}

		int m_Button;
	};

	class MouseEventButtonPressed : public MouseEventButton {
	public:
		MouseEventButtonPressed(int button) : MouseEventButton(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseEventButtonPressed: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseEventButtonReleased : public MouseEventButton {
		MouseEventButtonReleased(int button) : MouseEventButton(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseEventButtonReleased: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}