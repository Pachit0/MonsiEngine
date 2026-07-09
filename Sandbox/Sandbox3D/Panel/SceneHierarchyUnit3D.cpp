#include <imgui.h>

#include "SceneHierarchyUnit3D.h"
#include "Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "SceneCamera.h"

namespace Monsi {

	SceneHierarchyUnit3D::SceneHierarchyUnit3D(const Reference<Scene>& scene)
	{
		SetContext(scene);

	}

	void SceneHierarchyUnit3D::SetContext(const Reference<Scene>& scene)
	{
		m_Scene = scene;
		m_Selected = {};
	}

	void SceneHierarchyUnit3D::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (m_Scene)
		{
			for (auto entity : m_Scene->m_Registry.view<entt::entity>())
			{
				Entity ent{ entity, m_Scene.get() };
				if (ent.HasComponent<TagComponent>()) {
					DrawEntityNode(ent);
				}
			};
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
			m_Selected = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_Selected) {
			DrawComponents(m_Selected);
		}

		ImGui::End();
	}

	void SceneHierarchyUnit3D::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_Selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_Selected = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity")) {
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)1337, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}

	void SceneHierarchyUnit3D::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[512];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}
		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.05f);

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& cameraEntity = entity.GetComponent<CameraComponent>();
				
				const char* projectionType[] = { "Orthographic", "Perspective" };
				const char* currentProjection = projectionType[(int)cameraEntity.Camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjection)) {
					for (int i = 0; i < 2; i++) {
						bool selectedProjection = currentProjection == projectionType[i];
						if (ImGui::Selectable(projectionType[i], selectedProjection)) {
							currentProjection = projectionType[i];
							cameraEntity.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (selectedProjection) {
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (cameraEntity.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float orthographicSize = cameraEntity.Camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthographicSize)) {
						cameraEntity.Camera.SetOrthographicSize(orthographicSize);
					}
					float orthographicFar = cameraEntity.Camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthographicFar)) {
						cameraEntity.Camera.SetOrthographicFarClip(orthographicFar);
					}
					float orthographicNear = cameraEntity.Camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthographicNear)) {
						cameraEntity.Camera.SetOrthographicNearClip(orthographicNear);
					}
				}

				if (cameraEntity.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perspectiveFOV = cameraEntity.Camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("FOV", &perspectiveFOV)) {
						cameraEntity.Camera.SetPerspectiveFOV(perspectiveFOV);
					}
					float perspectiveFar = cameraEntity.Camera.GetPerspectiveFar();
					if (ImGui::DragFloat("Far", &perspectiveFar)) {
						cameraEntity.Camera.SetPerspectiveFar(perspectiveFar);
					}
					float perspectiveNear = cameraEntity.Camera.GetPerspectiveNear();
					if (ImGui::DragFloat("Near", &perspectiveNear)) {
						cameraEntity.Camera.SetPerspectiveNear(perspectiveNear);
					}
				}

				ImGui::TreePop();
			}
		}
	}

}