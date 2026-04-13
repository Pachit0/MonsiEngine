#pragma once

#include <Monsi.h>

class Chess : public Monsi::Layer {
public:
	Chess();
	virtual ~Chess() = default;

	void OnLayerAttach() override;
	void OnLayerUpdate(Monsi::TimeStep timestep) override;
	void OnLayerDetach() override;
	void OnLayerEvent(Monsi::Event& event) override;
	void OnImGuiDraw() override;

private:
	std::unordered_map<char, glm::vec4> s_TextureMap;
	Monsi::Reference<Monsi::Texture2D> m_Pieces;
	Monsi::Reference<Monsi::SubTexture2D> m_BalckPawn;
	Monsi::Reference<Monsi::SubTexture2D> m_WhitePawn;
	Monsi::OrthographicControl m_CameraControl;
};