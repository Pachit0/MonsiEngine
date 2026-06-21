#pragma once

#include <Monsi.h>

class ExampleLayer : public Monsi::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	void OnLayerAttach() override;
	void OnLayerUpdate(Monsi::TimeStep timestep) override;
	void OnLayerDetach() override;
	void OnImGuiDraw() override;
	void OnLayerEvent(Monsi::Event& event) override;

private:
	glm::vec2 m_ViewportSize;
	bool m_ViewportFocused;
	bool m_ViewportHovered;

	Monsi::Reference<Monsi::FrameBuffer> m_FrameBuffer;
	Monsi::Reference<Monsi::Model> m_Model;
	Monsi::Reference<Monsi::Model> m_ModelTwo;
	Monsi::SceneLighting currentFrameLighting;

	Monsi::PerspectiveControl m_CameraControl;
	Monsi::Reference<Monsi::Texture2D> m_MonsiTest;
	Monsi::Reference<Monsi::CubeMapTexture> m_SkyBoxTest;
	Monsi::Scope<Monsi::Mesh> m_SphereTest;

private: //fps counter
	float m_FPS = 0.0f;
	float m_FrameTimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	const float m_UpdateInterval = 0.5f;
};