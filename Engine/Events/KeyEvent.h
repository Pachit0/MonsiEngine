#pragma once

#include "Events.h"
#include "EventFormatter.h"

namespace Monsi {
	class ENGINE_API KeyEvent : public Event {

	public:
		inline int GetKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(CategoryInput | CategoryKeyboard)
	protected:
		KeyEvent(int keycode) : m_keyCode(keycode) {}
		int m_keyCode;
	};

	class ENGINE_API KeyEventPressed : public KeyEvent {
	public:
		KeyEventPressed(int keycode, bool repeat) : KeyEvent(keycode), m_Repeat(repeat) {}

		inline bool getRepeat() const { return m_Repeat; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyEventPressed: " << m_keyCode << (m_Repeat == 0 ? " Button not repeating!" : " Button repeating!");
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_Repeat = false;
	};

	class ENGINE_API KeyEventReleased : public KeyEvent {
	public:
		KeyEventReleased(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyEventReleased: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}