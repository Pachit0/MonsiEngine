#include "MonsiPch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Monsi {

	Renderer::SceneData* Renderer::m_Scene = new Renderer::SceneData;

	void Renderer::Init() {
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Begin(OrthographicCamera& camera) {
		m_Scene->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::End() {

	}

	void Renderer::Sumbit(const Reference<VertexArray>& vertexArray, const Reference<Shader>& shader, const glm::mat4 transform) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_ViewProjection", m_Scene->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}