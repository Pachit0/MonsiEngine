#include "MonsiPch.h"
#include "ShadowMapPass.h"
#include "RenderCommand.h"

namespace Monsi {

	void ShadowMapPass::Init()
	{
		m_ShadowMapShader = Shader::Create(SHADER_PATH "ShadowMapShader.glsl");
	}

	void ShadowMapPass::Shutdown()
	{
		m_ShadowMapShader.reset();
		Clear();
	}

	void ShadowMapPass::SubmitModel(const Reference<Model>& model, const glm::mat4& transform)
	{
		if (!model) return;

		ShadowCasterCommand cmd;
		cmd.ModelRef = model;
		cmd.Transform = transform;
		m_Commands.push_back(cmd);
	}

	void ShadowMapPass::SubmitMesh(const Mesh* meshPtr, const glm::mat4& transform)
	{
		if (!meshPtr) return;

		ShadowCasterCommand cmd;
		cmd.MeshPtr = meshPtr;
		cmd.Transform = transform;
		m_Commands.push_back(cmd);
	}

	void ShadowMapPass::ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap)
	{
		shadowMap->Resize(width, height);
	}

	void ShadowMapPass::Clear()
	{
		m_Commands.clear();
	}

	void ShadowMapPass::DrawMeshDepthOnly(const Mesh* meshPtr, const glm::mat4& transform)
	{
		if (!meshPtr) return;

		m_ShadowMapShader->setMat4("u_Transform", transform);

		auto& vao = meshPtr->GetVertexArray();
		vao->Bind();

		RenderCommand::DrawIndexedInstanced(vao, meshPtr->GetIndexCount(), 1);
	}

	void ShadowMapPass::DrawShadowMap(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap)
	{
		if (!shadowMap || m_Commands.empty()) return;

		shadowMap->Bind();
		RenderCommand::Clear();

		m_ShadowMapShader->Bind();
		m_ShadowMapShader->setMat4("u_LightSpaceMatrix", lightSpaceMatrix);

		for (const auto& cmd : m_Commands)
		{
			if (cmd.MeshPtr)
			{
				DrawMeshDepthOnly(cmd.MeshPtr, cmd.Transform);
			}
			else if (cmd.ModelRef)
			{
				for (const auto& mesh : cmd.ModelRef->GetMeshes())
					DrawMeshDepthOnly(&mesh, cmd.Transform);
			}
		}

		m_ShadowMapShader->Unbind();
		shadowMap->Unbind();
	}

}