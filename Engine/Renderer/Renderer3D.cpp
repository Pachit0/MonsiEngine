#include "MonsiPch.h"
#include "Renderer3D.h"
#include "CubePass.h"
#include "ModelPass.h"
#include "SkyBoxPass.h"


namespace Monsi {

	struct Renderer3DData {

		Reference<CubePass> Cube;
		Reference<ModelPass> Model;
		Reference<LightingBuffer> Lighting;
		Reference< SkyBoxPass> SkyBox;
		SceneLighting SceneLight;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.Cube = CreateReference<CubePass>();
		s_Data.Cube->Init();
		s_Data.Model = CreateReference<ModelPass>();
		s_Data.Model->Init();
		s_Data.Lighting = CreateReference<LightingBuffer>();
		s_Data.SkyBox = CreateReference<SkyBoxPass>();
		s_Data.SkyBox->Init();
	}

	void Renderer3D::Shutdown()
	{
		s_Data.Model->Shutdown();
		s_Data.Model.reset();

		s_Data.Cube->Shutdown();
		s_Data.Cube.reset();

		s_Data.SkyBox->Shutdown();
		s_Data.SkyBox.reset();
	}

	void Renderer3D::Begin3D(const PerspectiveControl& camera, const SceneLighting& lighting) //begin3D shouldn't have lighting passed here | I should consider adding a default
	{																						  //lighting so that it's placed here instead!!!!!!!!!!
		s_Data.Lighting->SetLighting(lighting);
		s_Data.Cube->BeginScene(camera.GetCamera().GetViewProjectionMatrix(), camera.GetCamera().GetPosition(), s_Data.Lighting);
		s_Data.Model->BeginScene(camera.GetCamera().GetViewProjectionMatrix(), camera.GetCamera().GetPosition(), s_Data.Lighting);
	}

	void Renderer3D::End3D()
	{
		s_Data.Cube->EndScene();
		s_Data.Model->EndScene();
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		s_Data.Cube->DrawCube(position, size, color, rotation);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, Reference<Texture2D> texture, const glm::vec3& rotation)
	{
		s_Data.Cube->DrawCube(position, size, texture, rotation);
	}

	void Renderer3D::DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data.Model->DrawModel(model, position, size, color);
	}

	void Renderer3D::DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		s_Data.Model->DrawModel(model, position, size, color, rotation);
	}

	void Renderer3D::DrawSkyBox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture)
	{
		s_Data.SkyBox->DrawSkybox(view, projection, skyboxTexture);
	}

	void Renderer3D::SetLighting(const SceneLighting& lighting)
	{
		s_Data.SceneLight = lighting;
		s_Data.Lighting->SetLighting(lighting);
	}
}