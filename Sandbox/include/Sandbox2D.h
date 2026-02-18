#pragma once

#include <Monsi.h>

class Sandbox2D : public Monsi::Layer{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	
	void OnLayerAttach() override;
	void OnLayerUpdate(Monsi::TimeStep timestep) override;
	void OnLayerDetach() override;
	void OnImGuiDraw() override;
	void OnLayerEvent(Monsi::Event& event) override;
private:
	Monsi::OrthographicControl m_CameraControl;

	Monsi::Reference<Monsi::VertexArray> m_SquareVertexArray;

	Monsi::Reference<Monsi::Shader> m_FlatColorShader;
	Monsi::Reference<Monsi::Texture2D> m_MonsiTest;

	struct TimeProfilerResult {
		const char* Name;
		float Time;
	};

	std::vector<TimeProfilerResult> m_TimeResults;

	glm::vec4 m_SquareColor = {0.6f, 0.4f, 0.2f, 1.0f};
};