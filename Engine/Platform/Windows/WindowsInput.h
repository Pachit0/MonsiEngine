#pragma once

#include "Input.h"

namespace Monsi {

	class WindowsInput : public Input {
	protected:
		virtual bool KeyPressedImpl(int keycode) override;
		virtual bool MouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	};

}