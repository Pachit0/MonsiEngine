#include "MonsiPch.h"
#include "ModelPass.h"
#include "RenderCommand.h"
#include "Lighting.h"
#include "Material.h"
#include <glm/ext/matrix_transform.hpp>

namespace Monsi {

	void ModelPass::Init()
	{
		m_InstanceBuffer = new ModelInstanceData[MaxInstances];

		m_Shader = Shader::Create(SHADER_PATH "ModelShader.glsl");

		m_InstanceVBO = VertexBuffer::Create(MaxInstances * sizeof(ModelInstanceData));

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
	}

	void ModelPass::Shutdown()
	{
		delete[] m_InstanceBuffer;
		m_InstanceBuffer = nullptr;
		m_RegisteredMeshes.clear();
	}

	void ModelPass::RegisterMesh(const Mesh* mesh)
	{
		auto& vao = mesh->GetVertexArray();
		vao->Bind();
		vao->AddVertexBuffer(m_InstanceVBO);
		m_RegisteredMeshes.insert(mesh);
	}

	void ModelPass::BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting)
	{
		m_ViewProjection = viewProj;
		m_Shader->Bind();
		m_Shader->setMat4("u_ViewProjection", m_ViewProjection);
		m_Shader->setVec3("u_ViewPos", viewPos);

		if (lighting)
		{
			lighting->Bind(m_Shader);
		}

		m_MeshBatches.clear();
		m_BufferCursor = m_InstanceBuffer;
	}

	void ModelPass::EndScene()
	{
		Flush();
	}

	void ModelPass::DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawModel(model, position, size, color, glm::vec3(0.0f));
	}

	void ModelPass::DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		const auto& meshes = model->GetMeshes();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			DrawMesh(&meshes[i], position, size, color, rotation);
		}
	}

	void ModelPass::DrawMesh(const Mesh* meshPtr, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		if (m_RegisteredMeshes.find(meshPtr) == m_RegisteredMeshes.end())
		{
			RegisterMesh(meshPtr);
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		if (rotation.x != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		if (rotation.y != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		if (rotation.z != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		auto& batch = m_MeshBatches[meshPtr];

		if (!batch.MeshPtr)
		{
			batch.MeshPtr = meshPtr;
		}

		batch.InstanceData.push_back({ transform, color });
	}

	void ModelPass::DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color)
	{
		const auto& meshes = model->GetMeshes();
		for (size_t i = 0; i < meshes.size(); i++)
			DrawMesh(&meshes[i], transform, color);
	}

	void ModelPass::DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color)
	{
		if (m_RegisteredMeshes.find(meshPtr) == m_RegisteredMeshes.end())
			RegisterMesh(meshPtr);

		auto& batch = m_MeshBatches[meshPtr];
		if (!batch.MeshPtr) batch.MeshPtr = meshPtr;
		batch.InstanceData.push_back({ transform, color });
	}

	void ModelPass::Flush()
	{
		if (m_MeshBatches.empty())
			return;

		m_Shader->Bind();
		m_Shader->setMat4("u_ViewProjection", m_ViewProjection);

		for (auto& [meshPtr, batch] : m_MeshBatches)
		{
			uint32_t count = (uint32_t)batch.InstanceData.size();
			if (count == 0)
				continue;

			auto& mesh = *batch.MeshPtr;
			const auto& material = mesh.GetMaterial();

			if (material)
			{
				material->Bind(m_Shader);

				if (!material->DiffuseMap)
				{
					m_WhiteTexture->Bind(0);
				}
			}
			else
			{
				m_WhiteTexture->Bind(0);
			}

			m_InstanceVBO->SetData(batch.InstanceData.data(), count * sizeof(ModelInstanceData));

			auto& vao = mesh.GetVertexArray();
			vao->Bind();

			RenderCommand::DrawIndexedInstanced(vao, mesh.GetIndexCount(), count);
		}
	}

}