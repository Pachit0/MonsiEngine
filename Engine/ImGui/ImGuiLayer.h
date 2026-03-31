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
		virtual ~ImGuiLayer();

		void OnLayerAttach() override;
		void OnLayerDetach() override;
		void OnImGuiDraw() override;
		void OnLayerEvent(Event& event) override;

		void Begin();
		void End();

		void SetImGuiEventState(bool state) { m_BlockImGuiEvents = state; }
	private:
		bool m_BlockImGuiEvents = true;
		float m_Time = 0.0f;
	};
}