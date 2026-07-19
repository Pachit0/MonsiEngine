#pragma once

#include <Monsi.h>
#include "SceneHierarchyUnit3D.h"

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

	Monsi::Reference<Monsi::Texture2D> m_MonsiTest;
	Monsi::Reference<Monsi::CubeMapTexture> m_SkyBoxTest;
	Monsi::Reference<Monsi::Mesh> m_SphereTest;
	Monsi::Reference<Monsi::Mesh> m_CubeTest;
	Monsi::Reference<Monsi::Mesh> m_TorusTest;
	Monsi::Reference<Monsi::Mesh> m_CylinderTest;
	Monsi::Reference<Monsi::Mesh> m_ConeTest;
	Monsi::Reference<Monsi::Mesh> m_QuadTest;
	Monsi::Reference<Monsi::Material> m_ShpereMaterial;

	Monsi::Reference<Monsi::Scene> m_Scene;
	Monsi::Entity m_CameraEntity;
	Monsi::Entity m_MainLightEntity;
	Monsi::Entity m_PointLightEntity;
	Monsi::Entity m_BackpackEntity;
	Monsi::Entity m_SponzaEntity;
	Monsi::Entity m_SphereEntity;
	Monsi::Entity m_CubeEntity;
	Monsi::Entity m_TorusEntity;
	Monsi::Entity m_ConeEntity;
	Monsi::Entity m_CylinderEntity;
	Monsi::Entity m_QuadEntity;

	Monsi::SceneHierarchyUnit3D m_Unit;
	glm::vec3 m_SpherePosition;

private: //fps counter
	float m_FPS = 0.0f;
	float m_FrameTimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	const float m_UpdateInterval = 0.5f;
};