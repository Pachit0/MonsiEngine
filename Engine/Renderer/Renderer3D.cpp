#include "MonsiPch.h"
#include "Renderer3D.h"
#include "ModelPass.h"
#include "SkyBoxPass.h"
#include "ShadowMap.h"
#include "ShadowMapPass.h"
#include "ColorPalette.h"

namespace Monsi {

	struct Renderer3DData {

		Reference<ModelPass> Model;
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
		s_Data.Model = CreateReference<ModelPass>();
		s_Data.Model->Init();
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
		s_Data.Model->Shutdown();
		s_Data.Model.reset();

		s_Data.SkyBox->Shutdown();
		s_Data.SkyBox.reset();

		s_Data.shadowMapPass->Shutdown();
		s_Data.shadowMapPass.reset();
	}

	void Renderer3D::Begin3D(const glm::mat4& viewProjection, const glm::vec3& cameraPosition)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Model->BeginScene(viewProjection, cameraPosition, s_Data.Lighting);
	}

	void Renderer3D::End3D()
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Model->EndScene();
		s_Data.Lighting->Clear();
		s_Data.shadowMapPass->Clear();
	}

	void Renderer3D::DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Model->SubmitModel(model, transform, color);
		s_Data.shadowMapPass->SubmitModel(model, transform);
	}

	void Renderer3D::DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();
		s_Data.Model->SubmitMesh(meshPtr, transform, color);
		s_Data.shadowMapPass->SubmitMesh(meshPtr, transform);
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
		s_Data.shadowMapPass->DrawShadowMap(projection * view, shadowMap);
	}

	void Renderer3D::SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap)
	{
		ENGINE_PROFILER_FUNCTION();
		if (shadowMap == nullptr) {
			s_Data.Model->SetShadowMapData(glm::mat4(1.0f), s_Data.shadow);
			return;
		}

		s_Data.Model->SetShadowMapData(lightSpaceMatrix, shadowMap);
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

		if (s_Data.Model)
		{
			const auto& modelStats = s_Data.Model->GetStats();
			stats.ModelDrawCalls = modelStats.DrawCalls;
			stats.ModelInstances = modelStats.Instances;
			stats.ModelTriangles = modelStats.Triangles;
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
		if (s_Data.Model)
			s_Data.Model->ResetStats();

		if (s_Data.shadowMapPass)
			s_Data.shadowMapPass->ResetStats();

		if (s_Data.SkyBox)
			s_Data.SkyBox->ResetStats();
	}

}