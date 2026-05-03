#pragma once

#include <Monsi.h>

namespace Monsi {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnLayerAttach() override;
		void OnLayerUpdate(TimeStep timestep) override;
		void OnLayerDetach() override;
		void OnImGuiDraw() override;
		void OnLayerEvent(Event& event) override;
	private:
		OrthographicControl m_CameraControl;

		Reference<Texture2D> m_MonsiTest;
		Reference<SubTexture2D> m_Chair;
		Reference<FrameBuffer> m_FrameBuffer;

		struct TimeProfilerResult {
			const char* Name;
			float Time;
		};

		Reference<Scene> m_ActiveScene;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused;
		bool m_ViewportHovered;

		std::unordered_map<char, Reference<SubTexture2D>> s_TextureMap;
	};

}