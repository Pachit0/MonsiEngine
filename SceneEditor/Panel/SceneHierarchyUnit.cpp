#include <imgui.h>
#include <imgui_internal.h>

#include "MonsiKeyCodes.h"
#include "Scripts/CameraControllerScript.h"
#include "SceneHierarchyUnit.h"
#include "RenderInitializator.h"
#include "MeshBuilder.h"
#include "Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "MeshInvalidationTracker.h"
#include "SceneCamera.h"
#include "PlatformUtilities.h"

namespace Monsi {

	struct Vec3ControlParams {
		const std::string& label;
		glm::vec3& values;
		float resetValue = 0.0f;
		float columnWidth = 100.0f;
		float minVal = 0.0f;
		float maxVal = 0.0f;
	};

	static void DrawVec3Control(Vec3ControlParams params)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(params.label.c_str());

		ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit;
		if (!ImGui::BeginTable("##Vec3ControlTable", 2, tableFlags)) {
			ImGui::PopID();
			return;
		}
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, params.columnWidth);
		ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthStretch);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.0f, 2.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 3.0f,3.0f });

		float lineHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(params.label.c_str());

		ImGui::TableSetColumnIndex(1);

		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize)) {
			params.values.x = params.resetValue;
		}

		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &params.values.x, 0.1f, params.minVal, params.maxVal, "%.4f");

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(1);

		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize)) {
			params.values.y = params.resetValue;
		}

		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &params.values.y, 0.1f, params.minVal, params.maxVal, "%.4f");

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(1);

		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize)) {
			params.values.z = params.resetValue;
		}

		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &params.values.z, 0.1f, params.minVal, params.maxVal, "%.4f");

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

			ImGui::PushID(label.c_str());
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
			ImGui::PopID();

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
				m_Scene->CreateEntityEmpty();
			}
			if (ImGui::MenuItem("Create Transform Entity")) {
				m_Scene->CreateEntity();
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
			m_Selected = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		static bool openModelSettingsModal = false;
		static Monsi::ModelImportSettings pendingSettings;

		if (m_Selected) {
			DrawComponents(m_Selected);

			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::BeginMenu("Add Component")) {
					if (ImGui::MenuItem("Camera")) {
						m_Selected.AddComponent<CameraComponent>();
						if (!m_Selected.HasComponent<TransformComponent>()) {
							m_Selected.AddComponent<TransformComponent>();
						}
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
							if (ImGui::BeginMenu("Mesh")) {
								if (ImGui::MenuItem("Sphere")) {
									m_Selected.AddComponent<MeshComponent>(MeshBuilder::Create(SphereParams{}, Monsi::CreateReference<Monsi::Material>()));
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Torus")) {
									m_Selected.AddComponent<MeshComponent>(MeshBuilder::Create(TorusParams{}, Monsi::CreateReference<Monsi::Material>()));
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Cube")) {
									m_Selected.AddComponent<MeshComponent>(MeshBuilder::Create(CubeParams{}, Monsi::CreateReference<Monsi::Material>()));
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Cylinder")) {
									m_Selected.AddComponent<MeshComponent>(MeshBuilder::Create(CylinderParams{}, Monsi::CreateReference<Monsi::Material>()));
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Cone")) {
									m_Selected.AddComponent<MeshComponent>(MeshBuilder::Create(ConeParams{}, Monsi::CreateReference<Monsi::Material>()));
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndMenu();
							}
						}

						bool canAddPointLight = !m_Selected.HasComponent<PointLightComponent>();
						bool canAddDirLight = !m_Selected.HasComponent<DirectionalLightComponent>();

						if (canAddPointLight || canAddDirLight) {
							if (ImGui::BeginMenu("Light")) {
								if (canAddPointLight) {
									if (ImGui::MenuItem("Point Light")) {
										m_Selected.AddComponent<PointLightComponent>();
										ImGui::CloseCurrentPopup();
									}
								}
								if (canAddDirLight || !m_Scene->m_Registry.view<DirectionalLightComponent>().empty()) {
									if (ImGui::MenuItem("Directional Light")) {
										m_Selected.AddComponent<DirectionalLightComponent>();
										ImGui::CloseCurrentPopup();
									}
								}
								ImGui::EndMenu();
							}
						}

						bool sceneHasShadowMap = !m_Scene->m_Registry.view<ShadowMapComponent>().empty();
						bool sceneHasSkyBox = !m_Scene->m_Registry.view<SkyBoxComponent>().empty();

						if (!sceneHasShadowMap && !m_Selected.HasComponent<ShadowMapComponent>()) {
							if (ImGui::MenuItem("ShadowMap")) {
								m_Selected.AddComponent<ShadowMapComponent>(ShadowMap::Create(4096, 4096));
								ImGui::CloseCurrentPopup();
							}
						}

						if (!sceneHasSkyBox) {
							if (ImGui::MenuItem("SkyBox")) {
								std::string path = FileDialogs::OpenFile("Image (*.png)\0*.png\0");
								if (!path.empty()) {
									auto cubeMap = CubeMapTexture::Create(path);
									m_Selected.AddComponent<SkyBoxComponent>(cubeMap, path);
								}
								ImGui::CloseCurrentPopup();
							}
							ImGui::SetItemTooltip("Only supports single file sky box from the UI. The API supports both!");
						}

						if (!m_Selected.HasComponent<TransformComponent>()) {
							if (ImGui::MenuItem("Transform")) {
								m_Selected.AddComponent<TransformComponent>();
								ImGui::CloseCurrentPopup();
							}
						}

						if (!m_Selected.HasComponent<TagComponent>()) {
							if (ImGui::MenuItem("Tag")) {
								m_Selected.AddComponent<TagComponent>();
								ImGui::CloseCurrentPopup();
							}
						}

						if (!m_Selected.HasComponent<ModelComponent>()) {
							if (ImGui::MenuItem("Model")) {
								openModelSettingsModal = true;
								ImGui::CloseCurrentPopup();
							}
						}
					}
					ImGui::EndMenu();
				}

				if (m_Selected.HasComponent<CameraComponent>()) {
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
				}

				ImGui::EndPopup();
			}

			if (openModelSettingsModal) {
				ImGui::OpenPopup("Model Import Settings");
				openModelSettingsModal = false;
			}

			if (ImGui::BeginPopupModal("Model Import Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("Configure Model Import Flags:");
				ImGui::Separator();

				ImGui::Checkbox("Flip UVs", &pendingSettings.FlipUVs);
				ImGui::Checkbox("Generate Smooth Normals", &pendingSettings.GenSmoothNormals);
				ImGui::Checkbox("Calculate Tangent Space", &pendingSettings.CalcTangentSpace);

				ImGui::Separator();

				if (ImGui::Button("Browse & Import", ImVec2(130, 0))) {
					std::string path = FileDialogs::OpenFile("3D Model (*.obj;*.fbx;*.gltf)\0*.obj;*.fbx;*.gltf\0");
					if (!path.empty()) {
						auto model = CreateReference<Monsi::Model>(path, pendingSettings);
						m_Selected.AddComponent<ModelComponent>(model);
					}
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(100, 0))) {
					ImGui::CloseCurrentPopup();
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
			if (entity.HasComponent<MeshComponent>())
			{
				MeshInvalidationTracker::MarkDirty();
			}

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
			strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);
			buffer[sizeof(buffer) - 1] = '\0';

			if (ImGui::InputText("##xxTag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		DrawComponent<TransformComponent>("Transform", entity, [&](auto& component) {
			DrawVec3Control({ "Translation", component.Translation });

			static Entity LastRotationEntity;
			static glm::vec3 EulerDegrees = glm::vec3(0.0f);

			if (entity != LastRotationEntity) {
				EulerDegrees = glm::degrees(glm::eulerAngles(component.Rotation));
				LastRotationEntity = entity;
			}

			glm::vec3 beforeEdit = EulerDegrees;
			DrawVec3Control({ "Rotation", EulerDegrees });

			if (EulerDegrees != beforeEdit) {
				component.Rotation = glm::quat(glm::radians(EulerDegrees));

				if (entity.HasComponent<NativeScriptComponent>()) {
					auto& nsc = entity.GetComponent<NativeScriptComponent>();
					if (auto* camScript = nsc.GetScriptAs<PerspectiveCameraControllerScript>()) {
						camScript->SetYawPitchRoll(EulerDegrees.y, EulerDegrees.x, EulerDegrees.z);
					}
				}
			}

			DrawVec3Control({ "Scale", component.Scale, 1.0f });
			}, false);

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			});

		DrawComponent<ShadowMapComponent>("ShadowMap", entity, [](auto& component) {
			ImGui::DragFloat("Size", &component.Settings.OrthoSize);
			ImGui::DragFloat("Distance", &component.Settings.LightDistance);
			ImGui::DragFloat("Near", &component.Settings.NearPlane);
			ImGui::DragFloat("Far", &component.Settings.FarPlane);

			const int resolutions[] = { 512, 1024, 2048, 4096, 8192 };
			const char* resolutionLabels[] = { "512x512", "1024x1024", "2048x2048", "4096x4096", "8192x8192" };

			std::string currentLabel = std::to_string(component.Settings.Width) + "x" + std::to_string(component.Settings.Height);
			int currentIndex = -1;

			for (int i = 0; i < 5; i++) {
				if (component.Settings.Width == resolutions[i] && component.Settings.Height == resolutions[i]) {
					currentIndex = i;
					currentLabel = resolutionLabels[i];
					break;
				}
			}

			if (ImGui::BeginCombo("Resolution", currentLabel.c_str())) {
				for (int i = 0; i < 5; i++) {
					bool isSelected = (currentIndex == i);
					if (ImGui::Selectable(resolutionLabels[i], isSelected)) {
						component.Settings.Width = resolutions[i];
						component.Settings.Height = resolutions[i];
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			});

		DrawComponent<SkyBoxComponent>("Skybox", entity, [](auto& component) {
			});

		DrawComponent<DirectionalLightComponent>("Directional Light", entity, [](auto& component) {
			DrawVec3Control({ .label = "Direction", .values = component.Direction, .minVal = -1.0f, .maxVal = 1.0f });
			ImGui::ColorEdit3("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Intensity", &component.Intensity);
			});

		DrawComponent<PointLightComponent>("Point Light", entity, [](auto& component) {
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

			ModelImportSettings settings = component.ModelAsset->GetModelSettings();
			bool settingsChanged = false;

			settingsChanged |= ImGui::Checkbox("Flip UVs", &settings.FlipUVs);
			settingsChanged |= ImGui::Checkbox("Gen Smooth Normals", &settings.GenSmoothNormals);
			settingsChanged |= ImGui::Checkbox("Calc Tangent Space", &settings.CalcTangentSpace);

			if (settingsChanged || ImGui::Button("Reload Model")) {
				std::string path = component.ModelAsset->GetFilePath();
				if (!path.empty()) {
					component.ModelAsset->LoadModel(path, settings);
				}
			}
			});

		DrawComponent<NativeScriptComponent>("Camera Controller Script", entity, [&](auto& component) {
			auto* camScript = component.template GetScriptAs<PerspectiveCameraControllerScript>();

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

		DrawComponent<CameraComponent>("Camera", entity, [this, entity](auto& component) {
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
			if (ImGui::Checkbox("Primary Camera", &component.Primary)) {
				if (component.Primary) {
					auto view = m_Scene->m_Registry.view<CameraComponent>();
					for (auto entt : view)
					{
						if (entt != entity)
						{
							auto& otherCamera = view.get<CameraComponent>(entt);
							otherCamera.Primary = false;
						}
					}
				}
			}
			});
	}

}