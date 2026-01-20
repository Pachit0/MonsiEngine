#pragma once
#include "Core.h"


namespace Monsi {

	class ENGINE_API Input {
	public:
		inline static bool KeyPressed(int keycode) { return s_Instance->KeyPressedImpl(keycode); }
		inline static bool MouseButtonPressed(int button) { return s_Instance->MouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool KeyPressedImpl(int keycode) = 0;
		virtual bool MouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};

}