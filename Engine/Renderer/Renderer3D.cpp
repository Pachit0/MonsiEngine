#include "MonsiPch.h"
#include "Renderer3D.h"
#include "ModelPass.h"
#include "SkyBoxPass.h"
#include "ColorPalette.h"


namespace Monsi {

	struct Renderer3DData {

		Reference<ModelPass> Model;
		Reference<LightingBuffer> Lighting;
		Reference< SkyBoxPass> SkyBox;
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
		s_Data.Lighting->SetLighting(s_Data.SceneLight);
	}

	void Renderer3D::Shutdown()
	{
		s_Data.Model->Shutdown();
		s_Data.Model.reset();

		s_Data.SkyBox->Shutdown();
		s_Data.SkyBox.reset();
	}

	void Renderer3D::Begin3D(const glm::mat4& viewProjection, const glm::vec3& cameraPosition)
	{
		s_Data.Model->BeginScene(viewProjection, cameraPosition, s_Data.Lighting);
	}

	void Renderer3D::End3D()
	{
		s_Data.Model->EndScene();
		s_Data.Lighting->Clear();
	}

	void Renderer3D::DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data.Model->DrawModel(model, transform, color);
	}

	void Renderer3D::DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data.Model->DrawMesh(meshPtr, transform, color);
	}

	void Renderer3D::DrawSkyBox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture)
	{
		s_Data.SkyBox->DrawSkybox(view, projection, skyboxTexture);
	}

	void Renderer3D::SetSceneLighting(const SceneLighting& lighting)
	{
		s_Data.SceneLight = lighting;
		s_Data.Lighting->SetLighting(lighting);
	}

}