#pragma once
#include "Core.h"


namespace Monsi {

	class ENGINE_API Input {
	public:
		static bool KeyPressed(int keycode);
		static bool MouseButtonPressed(int button);
		static bool MouseButtonReleased(int button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};

}