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

	Monsi::PerspectiveControl m_CameraControl;
	Monsi::Reference<Monsi::Texture2D> m_MonsiTest;
};