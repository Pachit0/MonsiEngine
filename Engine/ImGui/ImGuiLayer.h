#pragma once

#include "Layer.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ApplicationEvent.h"
#include "imgui.h"

namespace Monsi {
	enum class ImGuiTheme { Dark, Light, Classic, Cyan, Magenta, Red, Blue };

	class ImGuiLayer : public Layer {
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
		void SetTheme(ImGuiTheme theme);

	private:
		void SetDarkThemeColors();
		void SetLightThemeColors();
		void SetClassicThemeColors();
		void SetCyanThemeColors();
		void SetMagentaThemeColors();
		void SetRedThemeColors();
		void SetBlueThemeColors();

	private:
		bool m_BlockImGuiEvents = true;
		float m_Time = 0.0f;
		ImGuiTheme m_CurrentTheme = ImGuiTheme::Cyan;
	};
}