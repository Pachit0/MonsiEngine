#pragma once

#include <Monsi.h>
#include <glm/glm.hpp>

#include "SceneHierarchyUnit.h"

class GameLayer : public Monsi::Layer {
public:
	GameLayer();
	virtual ~GameLayer() = default;

	void OnLayerAttach() override;
	void OnLayerDetach() override;
	void OnLayerUpdate(Monsi::TimeStep timestep) override;
	void OnImGuiDraw() override;
	void OnLayerEvent(Monsi::Event& event) override;

private:
	bool OnKeyPressed(Monsi::KeyEventPressed& event);

private:
	glm::vec3 velocity{ 0.0f,0.0f,0.0f };
	glm::vec2 m_ViewportSize{ 0.0f, 0.0f };
	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	Monsi::Reference<Monsi::Scene> m_Scene;
	Monsi::Reference<Monsi::FrameBuffer> m_FrameBuffer;

	Monsi::Reference<Monsi::StaticModel> m_Model;

	// Default Scene Entities
	Monsi::Entity m_CameraEntity;
	Monsi::Entity m_DirectionalLightEntity;
	Monsi::Entity m_CubeEntity;
	Monsi::Entity m_ModelEntity;

	//ImGui Panel
	Monsi::SceneHierarchyUnit m_Unit;
};