#include "MonsiPch.h"
#include "Renderer3D.h"
#include "CubePass.h"
#include "ModelPass.h"

namespace Monsi {

	struct Renderer3DData {

		Reference<CubePass> Cube;
		Reference<ModelPass> Model;

	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.Cube = CreateReference<CubePass>();
		s_Data.Cube->Init();
		s_Data.Model = CreateReference<ModelPass>();
		s_Data.Model->Init();
	}

	void Renderer3D::Shutdown()
	{
		s_Data.Model->Shutdown();
		s_Data.Model.reset();

		s_Data.Cube->Shutdown();
		s_Data.Cube.reset();
	}

	void Renderer3D::Begin3D(const PerspectiveControl& camera)
	{
		s_Data.Cube->BeginScene(camera.GetCamera().GetViewProjectionMatrix());
		s_Data.Model->BeginScene(camera.GetCamera().GetViewProjectionMatrix());
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

}