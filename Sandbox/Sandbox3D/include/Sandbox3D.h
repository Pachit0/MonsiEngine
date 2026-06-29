#pragma once

#include <Monsi.h>

class Sandbox3D : public Monsi::Layer {
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

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
	Monsi::Reference<Monsi::Model> m_Backpack;

	Monsi::Reference<Monsi::Model> m_Sponza;
	
	Monsi::SceneLighting currentFrameLighting;

	Monsi::PerspectiveControl m_CameraControl;
	Monsi::Reference<Monsi::Texture2D> m_MonsiTest;
	Monsi::Reference<Monsi::CubeMapTexture> m_SkyBoxTest;
	Monsi::Scope<Monsi::Mesh> m_SphereTest;
	glm::vec3 m_SpherePosition;

	float m_DeltaTime;
	float m_lastFrame;

private: //fps counter
	float m_FPS = 0.0f;
	float m_FrameTimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	const float m_UpdateInterval = 0.5f;
};