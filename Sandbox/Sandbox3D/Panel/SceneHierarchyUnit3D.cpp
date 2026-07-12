#include <imgui.h>
#include <imgui_internal.h>

#include "SceneHierarchyUnit3D.h"
#include "Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "SceneCamera.h"

namespace Monsi {

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit;
		if (!ImGui::BeginTable("##Vec3ControlTable", 2, tableFlags)) {
			ImGui::PopID();
			return;
		}
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, columnWidth);
		ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthStretch);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.0f, 2.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

		float lineHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(label.c_str());

		ImGui::TableSetColumnIndex(1);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.6f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.85f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.35f, 0.35f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.4f");

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(1);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.6f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.85f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.35f, 0.35f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.4f");

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(1);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.6f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.85f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.35f, 0.35f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.4f");

		ImGui::PopStyleVar(3);

		ImGui::EndTable();

		ImGui::PopID();
	}

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
				auto& transform = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", transform.Translation);

				static Entity m_LastRotationEntity;
				static glm::vec3 m_CachedEulerDegrees;

				if (entity != m_LastRotationEntity) {
					m_CachedEulerDegrees = glm::degrees(glm::eulerAngles(transform.Rotation));
					m_LastRotationEntity = entity;
				}

				DrawVec3Control("Rotation", m_CachedEulerDegrees);
				transform.Rotation = glm::quat(glm::radians(m_CachedEulerDegrees));

				DrawVec3Control("Scale", transform.Scale);

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<SpriteRendererComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Color")) {
				auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
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