#include "MonsiPch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer2D.h"
#include "Renderer3D.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "RenderInitializator.h"
#include "ShadowMap.h"
#include "glad/glad.h"

namespace Monsi {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name, bool hasTransform)
	{
		Entity e{ m_Registry.create(), this };

		if (hasTransform)
			e.AddComponent<TransformComponent>();

		auto& tag = e.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return e;
	}

	Entity Scene::CreateEntityEmpty()
	{
		return { m_Registry.create(), this };
	}


	void Scene::RemoveEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(TimeStep timeStep)
	{
		SceneCamera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		glm::vec3 cameraPosition;

		RenderTypeEnum renderType = RenderSystem::GetActiveType();

		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScript)
				{
					if (!nativeScript.Instance)
					{
						nativeScript.Instance = nativeScript.InstantiateFuncPtr();
						nativeScript.Instance->m_Entity = Entity{ entity, this };
						nativeScript.Instance->OnCreate();
					}

					nativeScript.Instance->OnUpdate(timeStep);
				});
		}

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				cameraPosition = transform.Translation;
				break;
			}
		}

		if (!mainCamera) return;

		if (renderType == RenderTypeEnum::Renderer3D)
		{
			glm::vec3 cameraPos = cameraPosition;
			glm::mat4 viewProj = mainCamera->GetProjectionMatrix() * glm::inverse(cameraTransform);

			SceneLighting sceneLighting;
			auto DirectionalLightView = m_Registry.view<TransformComponent, DirectionalLightComponent>();
			for (auto entity : DirectionalLightView)
			{
				auto [transform, light] = DirectionalLightView.get<TransformComponent, DirectionalLightComponent>(entity);
				sceneLighting.MainLight.Direction = light.Direction;
				sceneLighting.MainLight.Color = light.Color;
				sceneLighting.MainLight.Intensity = light.Intensity;
			}

			auto PointLightView = m_Registry.view<TransformComponent, PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto [transform, light] = PointLightView.get<TransformComponent, PointLightComponent>(entity);
				glm::vec3 position = transform.Translation;
				sceneLighting.PointLights.push_back({ position, light.Color, light.Intensity, light.Radius });
			}

			Renderer3D::SetSceneLighting(sceneLighting);

			auto skyboxView = m_Registry.view<SkyBoxComponent>();
			for (auto entity : skyboxView)
			{
				auto& skybox = skyboxView.get<SkyBoxComponent>(entity);
				if (skybox.SkyBox)
				{
					glm::mat4 skyboxViewMatrix = glm::mat4(glm::mat3(glm::inverse(cameraTransform)));

					glm::mat4 projMatrix;
					if (mainCamera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
					{
						float aspectRatio = (m_ViewportHeight > 0) ? (float)m_ViewportWidth / (float)m_ViewportHeight : 1.0f;
						projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
					}
					else
					{
						projMatrix = mainCamera->GetProjectionMatrix();
					}

					skybox.SkyBox->DrawSkybox(skyboxViewMatrix, projMatrix, skybox.SkyboxTexture);
				}
			}

			Renderer3D::Begin3D(viewProj, cameraPos);

			auto meshGroup = m_Registry.view<TransformComponent, MeshComponent>();
			for (auto entity : meshGroup)
			{
				auto [transform, mesh] = meshGroup.get<TransformComponent, MeshComponent>(entity);
				Renderer3D::DrawMesh(mesh.MeshAsset.get(), transform.GetTransform(), glm::vec4(1.0f));
			}

			auto modelGroup = m_Registry.view<TransformComponent, ModelComponent>();
			for (auto entity : modelGroup)
			{
				auto [transform, model] = modelGroup.get<TransformComponent, ModelComponent>(entity);
				Renderer3D::DrawModel(model.ModelAsset, transform.GetTransform(), glm::vec4(1.0f));
			}

			auto shadowMapView = m_Registry.view<ShadowMapComponent>();
			for (auto entity : shadowMapView) {
				auto& shadowMapComp = shadowMapView.get<ShadowMapComponent>(entity);
				if (shadowMapComp.Shadow) {

					const auto& settings = shadowMapComp.Settings;

					if (m_ShadowMapFramebufferWidth != shadowMapComp.Settings.Width || m_ShadowMapFramebufferHeight != shadowMapComp.Settings.Height) {
						m_ShadowMapFramebufferHeight = shadowMapComp.Settings.Height;
						m_ShadowMapFramebufferWidth = shadowMapComp.Settings.Width;

						Renderer3D::ResizeShadowMap(settings.Width, settings.Height, shadowMapComp.Shadow);
					}

					glm::vec3 lightPos = cameraPosition - sceneLighting.MainLight.Direction * settings.LightDistance;

					glm::vec3 lightDir = glm::normalize(sceneLighting.MainLight.Direction);
					glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

					if (std::abs(lightDir.y) > 0.999f) {
						upVector = glm::vec3(0.0f, 0.0f, 1.0f);
					}

					glm::mat4 lightView = glm::lookAt(lightPos, cameraPosition, upVector);
					glm::mat4 lightProjection = glm::ortho(-settings.OrthoSize, settings.OrthoSize, -settings.OrthoSize, settings.OrthoSize, settings.NearPlane, settings.FarPlane);
					glm::mat4 lightSpaceMatrix = lightProjection * lightView;

					Renderer3D::DrawShadowMap(lightView, lightProjection, shadowMapComp.Shadow);
					Renderer3D::SetShadowMapData(lightSpaceMatrix, shadowMapComp.Shadow);
				}
			}

			Renderer3D::End3D();
		}
		else
		{
			Renderer2D::BeginScene2D(mainCamera->GetProjectionMatrix(), cameraTransform);

			auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : spriteGroup)
			{
				auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene2D();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	template<typename T>
	void Scene::OnAddComponent(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnAddComponent<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnAddComponent<TransformComponent>(Entity entity, TransformComponent& component) {}

	template<>
	void Scene::OnAddComponent<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template<>
	void Scene::OnAddComponent<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0) {
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	template<>
	void Scene::OnAddComponent<DirectionalLightComponent>(Entity entity, DirectionalLightComponent& component) {}

	template<>
	void Scene::OnAddComponent<PointLightComponent>(Entity entity, PointLightComponent& component) {}

	template<>
	void Scene::OnAddComponent<MeshComponent>(Entity entity, MeshComponent& component) {}

	template<>
	void Scene::OnAddComponent<ModelComponent>(Entity entity, ModelComponent& component) {}

	template<>
	void Scene::OnAddComponent<SkyBoxComponent>(Entity entity, SkyBoxComponent& component) {}

	template<>
	void Scene::OnAddComponent<ShadowMapComponent>(Entity entity, ShadowMapComponent& component) {
		m_ShadowMapFramebufferWidth = component.Settings.Width;
		m_ShadowMapFramebufferHeight = component.Settings.Height;
	}

	template<>
	void Scene::OnAddComponent<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

}