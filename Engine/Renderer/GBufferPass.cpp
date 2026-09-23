#include "MonsiPch.h"
#include "GBufferPass.h"
#include "RenderCommand.h"

namespace Monsi {

	void GBufferPass::Init()
	{
		m_GBufferShader = Shader::Create(SHADER_PATH "GBuffer.glsl");
	}

	void GBufferPass::Shutdown()
	{
		m_GBufferShader.reset();
	}

	void GBufferPass::Begin(const glm::mat4& view, const glm::mat4& projection, const Reference<FrameBuffer>& gBuffer)
	{
		if (!gBuffer) return;

		m_Stats = {};
		m_ActiveGBuffer = gBuffer;
		m_View = view;
		m_Projection = projection;

		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_PreviousFramebuffer);
		glGetIntegerv(GL_VIEWPORT, m_PreviousViewport);

		m_ActiveGBuffer->Bind();
		RenderCommand::Clear();

		m_GBufferShader->Bind();
		m_GBufferShader->setMat4("u_View", m_View);
		m_GBufferShader->setMat4("u_Projection", m_Projection);
	}

	void GBufferPass::SubmitMesh(const StaticMesh* meshPtr, const glm::mat4& transform)
	{
		if (!m_ActiveGBuffer || !meshPtr) return;

		glm::mat4 normalMatrix = glm::transpose(glm::inverse(m_View * transform));

		m_GBufferShader->setMat4("u_Model", transform);
		m_GBufferShader->setMat4("u_NormalMatrix", normalMatrix);

		const Reference<VertexArray>& vertexArray = meshPtr->GetVertexArray();
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, meshPtr->GetIndexCount());

		m_Stats.DrawCalls++;
		m_Stats.Triangles += meshPtr->GetIndexCount() / 3;
	}

	void GBufferPass::SubmitModel(const Reference<StaticModel>& model, const glm::mat4& transform)
	{
		if (!m_ActiveGBuffer || !model) return;

		for (const StaticMesh& mesh : model->GetMeshes())
		{
			SubmitMesh(&mesh, transform);
		}
	}

	void GBufferPass::End()
	{
		if (m_ActiveGBuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)m_PreviousFramebuffer);
			glViewport(m_PreviousViewport[0], m_PreviousViewport[1], m_PreviousViewport[2], m_PreviousViewport[3]);
		}

		m_ActiveGBuffer.reset();
	}

}