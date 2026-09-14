#include "MonsiPch.h"
#include "Renderer3D.h"
#include "MeshPass.h"
#include "AnimatedModelPass.h"
#include "SkyBoxPass.h"
#include "ShadowMap.h"
#include "ShadowMapPass.h"
#include "ColorPalette.h"

namespace Monsi {

	struct Renderer3DData {
		Reference<MeshPass> Mesh;
		Reference<AnimatedModelPass> AnimatedModel;
		Reference<LightingBuffer> Lighting;
		Reference<SkyBoxPass> SkyBox;
		Reference<ShadowMapPass> shadowMapPass;

		SceneLighting SceneLight;
		Reference<ShadowMap> shadow;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Mesh = CreateReference<MeshPass>();
		s_Data.Mesh->Init();

		s_Data.AnimatedModel = CreateReference<AnimatedModelPass>();
		s_Data.AnimatedModel->Init();

		s_Data.Lighting = CreateReference<LightingBuffer>();
		s_Data.SkyBox = CreateReference<SkyBoxPass>();
		s_Data.SkyBox->Init();

		s_Data.shadowMapPass = CreateReference<ShadowMapPass>();
		s_Data.shadowMapPass->Init();

		s_Data.Lighting->SetLighting(s_Data.SceneLight);
		s_Data.shadow = ShadowMap::Create(1, 1);
	}

	void Renderer3D::Shutdown()
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Mesh->Shutdown();
		s_Data.Mesh.reset();

		s_Data.AnimatedModel->Shutdown();
		s_Data.AnimatedModel.reset();

		s_Data.SkyBox->Shutdown();
		s_Data.SkyBox.reset();

		s_Data.shadowMapPass->Shutdown();
		s_Data.shadowMapPass.reset();

		s_Data.Lighting.reset();
	}

	void Renderer3D::Begin3D(const glm::mat4& viewProjection, const glm::vec3& cameraPosition)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Mesh->BeginScene(viewProjection, cameraPosition, s_Data.Lighting);
		s_Data.AnimatedModel->BeginScene(viewProjection, cameraPosition, s_Data.Lighting);
	}

	void Renderer3D::End3D()
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Mesh->EndScene();
		s_Data.AnimatedModel->EndScene();
		s_Data.Lighting->Clear();
		s_Data.shadowMapPass->Clear();
	}

	void Renderer3D::DrawModel(const Reference<StaticModel>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Mesh->SubmitModel(model, transform, color);
		s_Data.shadowMapPass->SubmitModel(model, transform);
	}

	void Renderer3D::DrawMesh(const StaticMesh* meshPtr, const glm::mat4& transform, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Mesh->SubmitMesh(meshPtr, transform, color);
		s_Data.shadowMapPass->SubmitMesh(meshPtr, transform);
	}

	void Renderer3D::DrawAnimatedModel(const Reference<AnimatedModel>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.AnimatedModel->SubmitModel(model, transform, color);
		s_Data.shadowMapPass->SubmitAnimatedModel(model, transform);
	}

	void Renderer3D::DrawSkyBox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.SkyBox->DrawSkybox(view, projection, skyboxTexture);
	}

	void Renderer3D::DrawShadowMap(const glm::mat4& view, const glm::mat4& projection, const Reference<ShadowMap>& shadowMap)
	{
		ENGINE_PROFILER_FUNCTION();
		if (shadowMap == nullptr) { return; }
		glm::mat4 lightSpaceMatrix = projection * view;
		s_Data.shadowMapPass->DrawShadowMap(lightSpaceMatrix, shadowMap);
	}

	void Renderer3D::SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap, float shadowIntensity)
	{
		ENGINE_PROFILER_FUNCTION();
		if (shadowMap == nullptr) {
			s_Data.Mesh->SetShadowMapData(glm::mat4(1.0f), s_Data.shadow, shadowIntensity);
			s_Data.AnimatedModel->SetShadowMapData(glm::mat4(1.0f), s_Data.shadow, shadowIntensity);
			return;
		}

		s_Data.Mesh->SetShadowMapData(lightSpaceMatrix, shadowMap, shadowIntensity);
		s_Data.AnimatedModel->SetShadowMapData(lightSpaceMatrix, shadowMap, shadowIntensity);
	}

	void Renderer3D::ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.shadowMapPass->ResizeShadowMap(width, height, shadowMap);
	}

	void Renderer3D::SetSceneLighting(const SceneLighting& lighting)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.SceneLight = lighting;
		s_Data.Lighting->SetLighting(lighting);
	}

	Renderer3DStats Renderer3D::GetStats()
	{
		Renderer3DStats stats;

		if (s_Data.Mesh)
		{
			const auto& meshStats = s_Data.Mesh->GetStats();
			stats.ModelDrawCalls = meshStats.DrawCalls;
			stats.ModelInstances = meshStats.Instances;
			stats.ModelTriangles = meshStats.Triangles;
		}

		if (s_Data.AnimatedModel)
		{
			const auto& animStats = s_Data.AnimatedModel->GetStats();
			stats.AnimatedModelDrawCalls = animStats.DrawCalls;
			stats.AnimatedModelInstances = animStats.Instances;
			stats.AnimatedModelTriangles = animStats.Triangles;
		}

		if (s_Data.shadowMapPass)
		{
			const auto& shadowStats = s_Data.shadowMapPass->GetStats();
			stats.ShadowDrawCalls = shadowStats.DrawCalls;
			stats.ShadowTriangles = shadowStats.Triangles;
		}

		if (s_Data.SkyBox)
		{
			const auto& skyboxStats = s_Data.SkyBox->GetStats();
			stats.SkyboxDrawCalls = skyboxStats.DrawCalls;
			stats.SkyboxTriangles = skyboxStats.Triangles;
		}

		return stats;
	}

	void Renderer3D::ResetStats()
	{
		if (s_Data.Mesh)
			s_Data.Mesh->ResetStats();

		if (s_Data.AnimatedModel)
			s_Data.AnimatedModel->ResetStats();

		if (s_Data.shadowMapPass)
			s_Data.shadowMapPass->ResetStats();

		if (s_Data.SkyBox)
			s_Data.SkyBox->ResetStats();
	}

}