#include "MonsiPch.h"
#include "ModelPass.h"
#include "RenderCommand.h"
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
	}

	void ModelPass::Shutdown()
	{
		delete[] m_InstanceBuffer;
		m_InstanceBuffer = nullptr;
		m_RegisteredModels.clear();
	}

	void ModelPass::RegisterModel(const Reference<Model>& model)
	{
		for (size_t i = 0; i < model->GetMeshes().size(); i++)
		{
			auto& vao = model->GetMeshes()[i].GetVertexArray();
			vao->Bind();
			vao->AddVertexBuffer(m_InstanceVBO);
		}
		m_RegisteredModels.insert(model.get());
	}

	void ModelPass::BeginScene(const glm::mat4& viewProjection)
	{
		m_ViewProjection = viewProjection;
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
		if (m_RegisteredModels.find(model.get()) == m_RegisteredModels.end())
		{
			RegisterModel(model);
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		if (rotation.x != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		if (rotation.y != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		if (rotation.z != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		const auto& meshes = model->GetMeshes();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			const Mesh* meshPtr = &meshes[i];
			auto& batch = m_MeshBatches[meshPtr];

			if (!batch.MeshPtr)
			{
				batch.MeshPtr = meshPtr;
			}

			batch.InstanceData.push_back({ transform, color });
		}
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

			const auto& textures = mesh.GetTextures();
			for (size_t i = 0; i < textures.size(); i++)
			{
				if (textures[i].type == "texture_diffuse")
				{
					textures[i].texture->Bind(0);
					break;
				}
			}

			m_InstanceVBO->SetData(batch.InstanceData.data(), count * sizeof(ModelInstanceData));

			auto& vao = mesh.GetVertexArray();
			vao->Bind();

			RenderCommand::DrawIndexedInstanced(vao, mesh.GetIndexCount(), count);
		}
	}

}