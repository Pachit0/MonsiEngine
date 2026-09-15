#include "MonsiPch.h"
#include "AnimatedModelPass.h"
#include "RenderCommand.h"
#include "Lighting.h"
#include "Material.h"
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

namespace Monsi {

	void AnimatedModelPass::Init()
	{
		m_Shader = Shader::Create(SHADER_PATH "AnimatedModelShader.glsl");

		m_InstanceVBO = VertexBuffer::Create(MaxInstances * sizeof(InstanceData));

		m_InstanceVBO->SetLayout({
			{ ShaderDataType::Float4, "a_InstanceTransform_row0", true },
			{ ShaderDataType::Float4, "a_InstanceTransform_row1", true },
			{ ShaderDataType::Float4, "a_InstanceTransform_row2", true },
			{ ShaderDataType::Float4, "a_InstanceTransform_row3", true },
			{ ShaderDataType::Float4, "a_InstanceColor",         true }
			});

		m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitePixel = 0xffffffff;
		m_WhiteTexture->modifyData(&whitePixel, sizeof(uint32_t));

		m_Shader->Bind();
		m_Shader->setInt("texture_diffuse1", 0);
		m_Shader->setInt("u_ShadowMap", ShadowMapTextureSlot);
	}

	void AnimatedModelPass::Shutdown()
	{
		m_MeshBatches.clear();
	}

	void AnimatedModelPass::ClearBatches()
	{
		m_MeshBatches.clear();
		m_FlushList.clear();
	}

	void AnimatedModelPass::RegisterMesh(const AnimatedMesh* mesh)
	{
		auto& vao = mesh->GetVertexArray();
		vao->Bind();
		vao->AddVertexBuffer(m_InstanceVBO);
	}

	void AnimatedModelPass::BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting)
	{
		m_Stats = {};

		m_ViewProjection = viewProj;
		m_Shader->Bind();
		m_Shader->setMat4("u_ViewProjection", m_ViewProjection);
		m_Shader->setVec3("u_ViewPos", viewPos);

		if (lighting)
		{
			lighting->Bind(m_Shader);
		}

		for (auto it = m_MeshBatches.begin(); it != m_MeshBatches.end(); )
		{
			if (it->second.LifetimeToken.expired())
			{
				it = m_MeshBatches.erase(it);
			}
			else
			{
				it->second.InstancesData.clear();
				++it;
			}
		}
	}

	void AnimatedModelPass::EndScene()
	{
		Flush();
	}

	void AnimatedModelPass::SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap, float shadowIntensity)
	{
		if (!shadowMap)
			return;

		m_Shader->Bind();
		m_Shader->setMat4("u_LightSpaceMatrix", lightSpaceMatrix);
		m_Shader->setFloat("u_ShadowIntensity", shadowIntensity);
		m_Shader->setFloat("u_ShadowMinLight", 0.20f);

		shadowMap->BindDepthTexture(ShadowMapTextureSlot);
		m_Shader->setInt("u_ShadowMap", ShadowMapTextureSlot);
	}

	void AnimatedModelPass::SubmitModel(const Reference<AnimatedModel>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		if (rotation.x != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		if (rotation.y != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		if (rotation.z != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		SubmitModel(model, transform, color);
	}

	void AnimatedModelPass::SubmitModel(const Reference<AnimatedModel>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		const auto& meshes = model->GetMeshes();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			const AnimatedMesh* meshPtr = &meshes[i];
			AnimatedBatchKey key{ meshPtr, model.get() };
			auto& batch = m_MeshBatches[key];

			if (batch.Key.MeshPtr != meshPtr || batch.LifetimeToken.expired())
			{
				RegisterMesh(meshPtr);

				batch.Key = key;
				batch.LifetimeToken = meshPtr->GetLifetimeToken();
				batch.InstancesData.clear();
				batch.InstancesData.reserve(DefaultBatchReserve);
				batch.WarnedOverflow = false;
			}

			batch.InstancesData.push_back({ transform, color });
		}
	}

	void AnimatedModelPass::Flush()
	{
		if (m_MeshBatches.empty())
			return;

		m_Shader->Bind();
		m_Shader->setMat4("u_ViewProjection", m_ViewProjection);

		m_FlushList.clear();
		for (auto& [key, batch] : m_MeshBatches)
		{
			if (batch.InstancesData.empty())
				continue;

			if (batch.LifetimeToken.expired())
				continue;

			m_FlushList.push_back(&batch);
		}

		std::sort(m_FlushList.begin(), m_FlushList.end(),
			[](const AnimatedMeshBatch* a, const AnimatedMeshBatch* b)
			{
				return a->Key.MeshPtr->GetMaterial().get() < b->Key.MeshPtr->GetMaterial().get();
			});

		Material* lastMaterial = nullptr;
		bool firstBatch = true;

		for (AnimatedMeshBatch* batchPtr : m_FlushList)
		{
			auto& batch = *batchPtr;
			uint32_t count = static_cast<uint32_t>(batch.InstancesData.size());

			if (count > MaxInstances)
			{
				if (!batch.WarnedOverflow)
				{
					ENGINE_LOG_WARN("AnimatedModelPass::Flush - mesh batch has {0} instances, exceeding MaxInstances ({1}). Clamping.", count, MaxInstances);
					batch.WarnedOverflow = true;
				}
				count = MaxInstances;
			}
			else
			{
				batch.WarnedOverflow = false;
			}

			auto& mesh = *batch.Key.MeshPtr;
			auto& model = *batch.Key.ModelPtr;
			Material* material = mesh.GetMaterial().get();

			const auto& boneTransforms = model.GetFinalBoneTransforms();
			for (size_t i = 0; i < boneTransforms.size(); ++i)
			{
				std::string uniformName = "u_FinalBoneMatrices[" + std::to_string(i) + "]";
				m_Shader->setMat4(uniformName.c_str(), boneTransforms[i]);
			}

			if (firstBatch || material != lastMaterial)
			{
				if (material)
				{
					material->Bind(m_Shader);

					if (!material->DiffuseMap)
					{
						m_WhiteTexture->Bind(0);
					}

					RenderCommand::SetCullFaces(!material->DoubleSided);
				}
				else
				{
					m_WhiteTexture->Bind(0);
					RenderCommand::SetCullFaces(true);
				}

				lastMaterial = material;
				firstBatch = false;
			}

			m_InstanceVBO->SetData(batch.InstancesData.data(), count * sizeof(InstanceData));

			auto& vao = mesh.GetVertexArray();
			vao->Bind();

			RenderCommand::DrawIndexedInstanced(vao, mesh.GetIndexCount(), count);

			m_Stats.DrawCalls++;
			m_Stats.Instances += count;
			m_Stats.Triangles += (mesh.GetIndexCount() / 3) * count;
		}
	}

}
