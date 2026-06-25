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

	Monsi::Reference<Monsi::Texture2D> m_MonsiTest;
	Monsi::Reference<Monsi::SubTexture2D> m_Chair;

	struct TimeProfilerResult {
		const char* Name;
		float Time;
	};

	std::vector<TimeProfilerResult> m_TimeResults;

	std::unordered_map<char, Monsi::Reference<Monsi::SubTexture2D>> s_TextureMap;
};