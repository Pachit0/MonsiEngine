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
		void OnImGuiDraw() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}