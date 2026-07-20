#include <imgui.h>
#include <imgui_internal.h>

#include "MonsiKeyCodes.h"
#include "Scripts/CameraControllerScript.h"
#include "SceneHierarchyUnit.h"
#include "RenderInitializator.h"
#include "MeshBuilder.h"
#include "Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "SceneCamera.h"

namespace Monsi {

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 3.0f,3.0f });

		float lineHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(label.c_str());

		ImGui::TableSetColumnIndex(1);

		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
		}

		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.4f");

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(1);

		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
		}

		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.4f");

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(1);

		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
		}

		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.4f");

		ImGui::PopStyleVar(4);

		ImGui::EndTable();

		ImGui::PopID();
	}

	template<typename T, typename Function>
	void DrawComponent(const std::string& label, Entity& entity, Function function, bool isRemovable = true) {
		if (entity.HasComponent<T>()) {
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 3.0f,3.0f });
			float lineHeight = ImGui::GetFrameHeight();

			ImGui::Separator();

			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
			auto& component = entity.GetComponent<T>();

			ImGui::PopStyleVar(1);

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.25f);
			if (ImGui::Button("...")) {
				ImGui::OpenPopup("Settings");
			}

			bool RemoveComponent = false;
			if (ImGui::BeginPopup("Settings")) {
				if (isRemovable) {
					if (ImGui::MenuItem("Remove Component")) {
						RemoveComponent = true;
					}
				}
				ImGui::EndPopup();
			}

			if (opened) {
				function(component);
				ImGui::TreePop();
			}

			if (RemoveComponent && isRemovable) {
				entity.RemoveComponent<T>();
			}
		}
	}

	SceneHierarchyUnit::SceneHierarchyUnit(const Reference<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyUnit::SetContext(const Reference<Scene>& scene)
	{
		m_Scene = scene;
		m_Selected = {};
	}

	void SceneHierarchyUnit::OnImGuiRender()
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

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Empty Entity")) {
				m_Scene->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
			m_Selected = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_Selected) {
			DrawComponents(m_Selected);

			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::BeginMenu("Add Component")) {
					if (ImGui::MenuItem("Camera")) {
						m_Selected.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}

					if (RenderSystem::GetActiveType() == RenderTypeEnum::Renderer2D) {
						if (ImGui::MenuItem("Sprite Renderer")) {
							m_Selected.AddComponent<SpriteRendererComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					else {
						if (!m_Selected.HasComponent<MeshComponent>()) {
							if (ImGui::MenuItem("Sphere")) {
								m_Selected.AddComponent<MeshComponent>(MeshBuilder::CreateSphere(1.0f, 32, 32, Monsi::CreateReference<Monsi::Material>()));
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::MenuItem("Torus")) {
								m_Selected.AddComponent<MeshComponent>(MeshBuilder::CreateTorus(2.5f, 0.5f, 32, 16, Monsi::CreateReference<Monsi::Material>()));
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::MenuItem("Cube")) {
								m_Selected.AddComponent<MeshComponent>(MeshBuilder::CreateCube(1.0f, Monsi::CreateReference<Monsi::Material>()));
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::MenuItem("Cylinder")) {
								m_Selected.AddComponent<MeshComponent>(MeshBuilder::CreateCylinder(2.5f, 8, 32, Monsi::CreateReference<Monsi::Material>()));
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::MenuItem("Cone")) {
								m_Selected.AddComponent<MeshComponent>(MeshBuilder::CreateCone(2.5f, 8, 32, Monsi::CreateReference<Monsi::Material>()));
								ImGui::CloseCurrentPopup();
							}
						}
					}
					ImGui::EndMenu();
				}

				if (!m_Selected.HasComponent<NativeScriptComponent>()) {
					if (ImGui::BeginMenu("Add Script")) {
						if (m_Selected.HasComponent<CameraComponent>()) {

							if (ImGui::MenuItem("Camera Controller")) {
								m_Selected.AddComponent<Monsi::NativeScriptComponent>().Bind<Monsi::PerspectiveCameraControllerScript>();
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndMenu();
					}
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyUnit::DrawEntityNode(Entity entity)
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
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)1337, flags, tag.c_str());
			if (opened) {
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Scene->RemoveEntity(entity);
			if (m_Selected == entity)
				m_Selected = {};
		}

	}

	void SceneHierarchyUnit::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[512];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##xxTag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		DrawComponent<TransformComponent>("Transform", entity, [&](auto& component) {
			DrawVec3Control("Translation", component.Translation);

			static Entity LastRotationEntity;
			static glm::vec3 EulerDegrees = glm::vec3(0.0f);

			if (entity != LastRotationEntity) {
				EulerDegrees = glm::degrees(glm::eulerAngles(component.Rotation));
				LastRotationEntity = entity;
			}

			glm::vec3 beforeEdit = EulerDegrees;
			DrawVec3Control("Rotation", EulerDegrees);

			if (EulerDegrees != beforeEdit) {
				component.Rotation = glm::quat(glm::radians(EulerDegrees));

				if (entity.HasComponent<NativeScriptComponent>()) {
					auto& nsc = entity.GetComponent<NativeScriptComponent>();
					if (auto* camScript = nsc.GetScriptAs<PerspectiveCameraControllerScript>()) {
						camScript->SetYawPitchRoll(EulerDegrees.y, EulerDegrees.x, EulerDegrees.z);
					}
				}
			}

			DrawVec3Control("Scale", component.Scale);
			}, false);

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			});

		DrawComponent<SkyBoxComponent>("Skybox", entity, [](auto& component) {
			ImGui::Text("A cute skybox");
			});

		DrawComponent<DirectionalLightComponent>("Directional Light", entity, [entity](auto& component) {
			DrawVec3Control("Direction", component.Direction);
			ImGui::ColorEdit3("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Intensity", &component.Intensity);
			});

		DrawComponent<PointLightComponent>("Point Light", entity, [entity](auto& component) {
			ImGui::ColorEdit3("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Radius", &component.Radius);
			ImGui::DragFloat("Intensity", &component.Intensity);
			});

		DrawComponent<MeshComponent>("Mesh", entity, [](auto& component) {
			auto& material = component.MeshAsset->GetMaterial();
			ImGui::ColorEdit3("Ambient Color", glm::value_ptr(material->AmbientColor));
			ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(material->DiffuseColor));
			ImGui::ColorEdit3("Specular Color", glm::value_ptr(material->SpecularColor));
			ImGui::DragFloat("Shininess", &material->Shininess);
			});

		DrawComponent<ModelComponent>("Model", entity, [](auto& component) {
			auto& materialVector = component.ModelAsset->GetMeshes();
			int count = materialVector.size();
			ImGui::Text("Number of meshes used: %d", count);
			//TODO everything else that should go here (I don't even know, will see some other day :3)
			});

		DrawComponent<NativeScriptComponent>("Camera Controller Script", entity, [&](auto& component) {
			auto* camScript = component.GetScriptAs<PerspectiveCameraControllerScript>();

			if (!camScript) {
				ImGui::TextDisabled("(script type has no exposed properties)");
				return;
			}

			const char* lookModeNames[] = { "Right Click", "FPS" };
			int currentMode = (int)camScript->GetLookMode();

			if (ImGui::BeginCombo("Look Mode", lookModeNames[currentMode])) {
				for (int i = 0; i < 2; i++) {
					bool selected = (currentMode == i);
					if (ImGui::Selectable(lookModeNames[i], selected)) {
						camScript->SetLookMode((PerspectiveCameraControllerScript::LookMode)i);
					}
					if (selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			glm::vec3 scriptYawPitchRoll = camScript->GetYawPitchRoll();
			ImGui::Text("Yaw: %.2f  Pitch: %.2f  Roll: %.2f", scriptYawPitchRoll.x, scriptYawPitchRoll.y, scriptYawPitchRoll.z);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			const char* projectionType[] = { "Orthographic", "Perspective" };
			const char* currentProjection = projectionType[(int)component.Camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjection)) {
				for (int i = 0; i < 2; i++) {
					bool selectedProjection = currentProjection == projectionType[i];
					if (ImGui::Selectable(projectionType[i], selectedProjection)) {
						currentProjection = projectionType[i];
						component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}
					if (selectedProjection) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthographicSize = component.Camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthographicSize)) {
					component.Camera.SetOrthographicSize(orthographicSize);
				}
				float orthographicFar = component.Camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthographicFar)) {
					component.Camera.SetOrthographicFarClip(orthographicFar);
				}
				float orthographicNear = component.Camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthographicNear)) {
					component.Camera.SetOrthographicNearClip(orthographicNear);
				}
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float perspectiveFOV = component.Camera.GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &perspectiveFOV)) {
					component.Camera.SetPerspectiveFOV(perspectiveFOV);
				}
				float perspectiveFar = component.Camera.GetPerspectiveFar();
				if (ImGui::DragFloat("Far", &perspectiveFar)) {
					component.Camera.SetPerspectiveFar(perspectiveFar);
				}
				float perspectiveNear = component.Camera.GetPerspectiveNear();
				if (ImGui::DragFloat("Near", &perspectiveNear)) {
					component.Camera.SetPerspectiveNear(perspectiveNear);
				}
			}
			});
	}

}