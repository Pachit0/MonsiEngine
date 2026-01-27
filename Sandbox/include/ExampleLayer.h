#pragma once

#include "Monsi.h"

class ExampleLayer : public Monsi::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	void OnLayerAttach() override;
	void OnLayerUpdate() override;
	void OnLayerDetach() override;
	void OnImGuiDraw() override;
	void OnLayerEvent(Monsi::Event& event) override;
private:
	std::shared_ptr<Monsi::Shader> m_Shader;
	std::shared_ptr < Monsi::VertexArray> m_VertexArray;

	std::shared_ptr < Monsi::Shader> m_SingleColorShader;
	std::shared_ptr < Monsi::VertexArray> m_SquareVertexArray;
};