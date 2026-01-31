#pragma once

#include "Monsi.h"

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
	Monsi::ShaderLibrary m_ShaderLibrary;
	Monsi::Reference<Monsi::Shader> m_Shader;
	Monsi::Reference<Monsi::VertexArray> m_VertexArray;

	Monsi::Reference<Monsi::Shader> m_SingleColorShader, m_TextureShader;
	Monsi::Reference<Monsi::VertexArray> m_SquareVertexArray;

	Monsi::Reference<Monsi::Texture2D> m_Texture, m_MonsiTest;

	Monsi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;
	float m_CameraRotation;
	float m_CameraRotationSpeed;

	glm::vec3 m_SquareColor;
};