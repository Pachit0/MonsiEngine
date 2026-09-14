#include "MonsiPch.h"
#include "ShadowMapPass.h"
#include "RenderCommand.h"

namespace Monsi {

	void ShadowMapPass::Init()
	{
		m_ShadowMapShader = Shader::Create(SHADER_PATH "ShadowMapShader.glsl");
		m_AnimatedShadowMapShader = Shader::Create(SHADER_PATH "AnimatedShadowMapShader.glsl");
	}

	void ShadowMapPass::Shutdown()
	{
		m_ShadowMapShader.reset();
		m_AnimatedShadowMapShader.reset();
		Clear();
	}

	void ShadowMapPass::SubmitModel(const Reference<StaticModel>& model, const glm::mat4& transform)
	{
		if (!model) return;

		StaticShadowCasterCommand cmd;
		cmd.ModelRef = model;
		cmd.Transform = transform;
		m_StaticCommands.push_back(cmd);
	}

	void ShadowMapPass::SubmitMesh(const StaticMesh* meshPtr, const glm::mat4& transform)
	{
		if (!meshPtr) return;

		StaticShadowCasterCommand cmd;
		cmd.MeshPtr = meshPtr;
		cmd.Transform = transform;
		m_StaticCommands.push_back(cmd);
	}

	void ShadowMapPass::SubmitAnimatedModel(const Reference<AnimatedModel>& model, const glm::mat4& transform)
	{
		if (!model) return;

		AnimatedShadowCasterCommand cmd;
		cmd.ModelRef = model;
		cmd.Transform = transform;
		m_AnimatedCommands.push_back(cmd);
	}

	void ShadowMapPass::ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap)
	{
		shadowMap->Resize(width, height);
	}

	void ShadowMapPass::Clear()
	{
		m_StaticCommands.clear();
		m_AnimatedCommands.clear();
	}

	void ShadowMapPass::DrawMeshDepthOnly(const StaticMesh* meshPtr, const glm::mat4& transform)
	{
		if (!meshPtr) return;

		m_ShadowMapShader->setMat4("u_Transform", transform);

		auto& vao = meshPtr->GetVertexArray();
		vao->Bind();

		RenderCommand::DrawIndexedInstanced(vao, meshPtr->GetIndexCount(), 1);

		m_Stats.DrawCalls++;
		m_Stats.Triangles += meshPtr->GetIndexCount() / 3;
	}

	void ShadowMapPass::DrawAnimatedMeshDepthOnly(const AnimatedMesh* meshPtr, const AnimatedModel* modelPtr, const glm::mat4& transform)
	{
		if (!meshPtr || !modelPtr) return;

		m_AnimatedShadowMapShader->setMat4("u_Transform", transform);

		const auto& boneTransforms = modelPtr->GetFinalBoneTransforms();
		for (size_t i = 0; i < boneTransforms.size(); ++i)
		{
			std::string uniformName = "u_FinalBoneMatrices[" + std::to_string(i) + "]";
			m_AnimatedShadowMapShader->setMat4(uniformName.c_str(), boneTransforms[i]);
		}

		auto& vao = meshPtr->GetVertexArray();
		vao->Bind();

		RenderCommand::DrawIndexedInstanced(vao, meshPtr->GetIndexCount(), 1);

		m_Stats.DrawCalls++;
		m_Stats.Triangles += meshPtr->GetIndexCount() / 3;
	}

	void ShadowMapPass::DrawShadowMap(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap)
	{
		m_Stats = {};

		if (!shadowMap || (m_StaticCommands.empty() && m_AnimatedCommands.empty())) return;

		shadowMap->Bind();
		RenderCommand::Clear();

		if (!m_StaticCommands.empty())
		{
			m_ShadowMapShader->Bind();
			m_ShadowMapShader->setMat4("u_LightSpaceMatrix", lightSpaceMatrix);

			for (const auto& cmd : m_StaticCommands)
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
		}

		if (!m_AnimatedCommands.empty())
		{
			m_AnimatedShadowMapShader->Bind();
			m_AnimatedShadowMapShader->setMat4("u_LightSpaceMatrix", lightSpaceMatrix);

			for (const auto& cmd : m_AnimatedCommands)
			{
				if (cmd.ModelRef)
				{
					for (const auto& mesh : cmd.ModelRef->GetMeshes())
					{
						DrawAnimatedMeshDepthOnly(&mesh, cmd.ModelRef.get(), cmd.Transform);
					}
				}
			}

			m_AnimatedShadowMapShader->Unbind();
		}

		shadowMap->Unbind();
	}

}