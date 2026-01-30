#include "MonsiPch.h"
#include "Renderer.h"
#include "Camera.h"

namespace Monsi {

	Renderer::SceneData* Renderer::m_Scene = new Renderer::SceneData;

	void Renderer::Begin(OrthographicCamera& camera) {
		m_Scene->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::End() {

	}

	void Renderer::Sumbit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader) {
		shader->Bind();
		shader->setMat4("u_ViewProjection", m_Scene->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}