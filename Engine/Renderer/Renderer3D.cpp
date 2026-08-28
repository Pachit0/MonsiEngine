#include "MonsiPch.h"
#include "Renderer3D.h"
#include "ModelPass.h"
#include "SkyBoxPass.h"
#include "ShadowMapPass.h"
#include "ColorPalette.h"

namespace Monsi {

	struct Renderer3DData {

		Reference<ModelPass> Model;
		Reference<LightingBuffer> Lighting;
		Reference<SkyBoxPass> SkyBox;
		Reference<ShadowMapPass> ShadowMap;
		SceneLighting SceneLight;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.Model = CreateReference<ModelPass>();
		s_Data.Model->Init();
		s_Data.Lighting = CreateReference<LightingBuffer>();
		s_Data.SkyBox = CreateReference<SkyBoxPass>();
		s_Data.SkyBox->Init();
		s_Data.ShadowMap = CreateReference<ShadowMapPass>();
		s_Data.ShadowMap->Init();
		s_Data.Lighting->SetLighting(s_Data.SceneLight);
	}

	void Renderer3D::Shutdown()
	{
		s_Data.Model->Shutdown();
		s_Data.Model.reset();

		s_Data.SkyBox->Shutdown();
		s_Data.SkyBox.reset();

		s_Data.ShadowMap->Shutdown();
		s_Data.ShadowMap.reset();
	}

	void Renderer3D::Begin3D(const glm::mat4& viewProjection, const glm::vec3& cameraPosition)
	{
		s_Data.Model->BeginScene(viewProjection, cameraPosition, s_Data.Lighting);
	}

	void Renderer3D::End3D()
	{
		s_Data.Model->EndScene();
		s_Data.Lighting->Clear();
		s_Data.ShadowMap->Clear();
	}

	void Renderer3D::DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data.Model->DrawModel(model, transform, color);
		s_Data.ShadowMap->SubmitModel(model, transform);
	}

	void Renderer3D::DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data.Model->DrawMesh(meshPtr, transform, color);
		s_Data.ShadowMap->SubmitMesh(meshPtr, transform);
	}

	void Renderer3D::DrawSkyBox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture)
	{
		s_Data.SkyBox->DrawSkybox(view, projection, skyboxTexture);
	}

	void Renderer3D::DrawShadowMap(const glm::mat4& view, const glm::mat4& projection, const Reference<ShadowMap>& shadowMap)
	{
		s_Data.ShadowMap->DrawShadowMap(projection * view, shadowMap);
	}

	void Renderer3D::SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap)
	{
		s_Data.Model->SetShadowMapData(lightSpaceMatrix, shadowMap);
	}

	void Renderer3D::SetSceneLighting(const SceneLighting& lighting)
	{
		s_Data.SceneLight = lighting;
		s_Data.Lighting->SetLighting(lighting);
	}

}