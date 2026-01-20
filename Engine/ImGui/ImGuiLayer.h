#pragma once

#include "Layer.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ApplicationEvent.h"
#include "imgui.h"

namespace Monsi {

	class ENGINE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnLayerAttach() override;
		void OnLayerDetach() override;
		void OnLayerUpdate() override;
		void OnLayerEvent(Event& event) override;
	private:
		bool MouseButtonPressedEvent(MouseEventButtonPressed& event);
		bool MouseButtonReleasedEvent(MouseEventButtonReleased& event);
		bool MouseMovedEvent(MouseEventMoved& event);
		bool MouseScrolledEvent(MouseEventScrolled& event);
		bool KeyReleasedEvent(KeyEventReleased& event);
		bool KeyPressedEvent(KeyEventPressed& event);
		bool WinResizedEvent(WindowResizeEvent& event);
		bool KeyTypedEvent(KeyEventTyped& event);
		ImGuiKey ToImGuiKey(int keycode);
	private:
		float m_Time = 0.0f;
	};
}