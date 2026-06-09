#include "MonsiPch.h"
#include "CubePass.h"
#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>

namespace Monsi {

	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	void CubePass::Init()
	{
		m_CubeVA = VertexArray::Create();

		CubeVertex vertices[24] =
		{
			{{-0.5f,-0.5f, 0.5f}, {0.0f,0.0f}},
			{{ 0.5f,-0.5f, 0.5f}, {1.0f,0.0f}},
			{{ 0.5f, 0.5f, 0.5f}, {1.0f,1.0f}},
			{{-0.5f, 0.5f, 0.5f}, {0.0f,1.0f}},

			{{ 0.5f,-0.5f, 0.5f}, {0.0f,0.0f}},
			{{ 0.5f,-0.5f,-0.5f}, {1.0f,0.0f}},
			{{ 0.5f, 0.5f,-0.5f}, {1.0f,1.0f}},
			{{ 0.5f, 0.5f, 0.5f}, {0.0f,1.0f}},

			{{ 0.5f,-0.5f,-0.5f}, {0.0f,0.0f}},
			{{-0.5f,-0.5f,-0.5f}, {1.0f,0.0f}},
			{{-0.5f, 0.5f,-0.5f}, {1.0f,1.0f}},
			{{ 0.5f, 0.5f,-0.5f}, {0.0f,1.0f}},

			{{-0.5f,-0.5f,-0.5f}, {0.0f,0.0f}},
			{{-0.5f,-0.5f, 0.5f}, {1.0f,0.0f}},
			{{-0.5f, 0.5f, 0.5f}, {1.0f,1.0f}},
			{{-0.5f, 0.5f,-0.5f}, {0.0f,1.0f}},

			{{-0.5f, 0.5f, 0.5f}, {0.0f,0.0f}},
			{{ 0.5f, 0.5f, 0.5f}, {1.0f,0.0f}},
			{{ 0.5f, 0.5f,-0.5f}, {1.0f,1.0f}},
			{{-0.5f, 0.5f,-0.5f}, {0.0f,1.0f}},

			{{-0.5f,-0.5f,-0.5f}, {0.0f,0.0f}},
			{{ 0.5f,-0.5f,-0.5f}, {1.0f,0.0f}},
			{{ 0.5f,-0.5f, 0.5f}, {1.0f,1.0f}},
			{{-0.5f,-0.5f, 0.5f}, {0.0f,1.0f}}
		};

		glm::vec3 normals[24] =
		{
			{ 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f},
			{ 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f},
			{ 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f},
			{-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f},
			{ 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f},
			{ 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}
		};

		uint32_t indices[36] =
		{
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		m_CubeVertexBuffer = VertexBuffer::Create((float*)vertices, sizeof(vertices));
		m_CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_CubeVA->AddVertexBuffer(m_CubeVertexBuffer);

		m_InstanceVertexBuffer = VertexBuffer::Create(MaxCubes * sizeof(CubeInstanceData));
		m_InstanceVertexBuffer->SetLayout({
			{ ShaderDataType::Float4, "a_MtxRow0",  true },
			{ ShaderDataType::Float4, "a_MtxRow1",  true },
			{ ShaderDataType::Float4, "a_MtxRow2",  true },
			{ ShaderDataType::Float4, "a_MtxRow3",  true },
			{ ShaderDataType::Float4, "a_Color",    true },
			{ ShaderDataType::Float,  "a_TexIndex", true }
			});
		m_CubeVA->AddVertexBuffer(m_InstanceVertexBuffer);

		Reference<VertexBuffer> normalBuffer = VertexBuffer::Create((float*)normals, sizeof(normals));
		normalBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Normal" }
			});
		m_CubeVA->AddVertexBuffer(normalBuffer);

		m_CubeIB = IndexBuffer::Create(indices, 36);
		m_CubeVA->SetIndexBuffer(m_CubeIB);

		m_InstanceBufferBegin = new CubeInstanceData[MaxCubes];
		m_InstanceBufferItr = m_InstanceBufferBegin;

		m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitePixel = 0xffffffff;
		m_WhiteTexture->modifyData(&whitePixel, sizeof(uint32_t));

		m_TextureSlots[0] = m_WhiteTexture;
		m_TextureSlotIndex = 1;

		m_TextureShader = Shader::Create(SHADER_PATH "ObjectShader.glsl");

		int samplers[MaxTextureSlots];
		for (uint32_t i = 0; i < MaxTextureSlots; i++)
			samplers[i] = i;

		m_TextureShader->Bind();
		m_TextureShader->setIntArray("u_Textures", samplers, MaxTextureSlots);
	}

	void CubePass::Shutdown()
	{
		delete[] m_InstanceBufferBegin;
		m_InstanceBufferBegin = nullptr;
		m_InstanceBufferItr = nullptr;
	}

	void CubePass::BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting)
	{
		m_ViewProjection = viewProj;

		m_TextureShader->Bind();
		m_TextureShader->setMat4("u_ViewProjection", m_ViewProjection);
		m_TextureShader->setVec3("u_ViewPos", viewPos);

		if (lighting)
		{
			lighting->Bind(m_TextureShader);
		}

		m_InstanceBufferItr = m_InstanceBufferBegin;
		m_TextureSlotIndex = 1;
	}

	void CubePass::EndScene()
	{
		Flush();
	}

	void CubePass::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		uint32_t currentInstances = m_InstanceBufferItr - m_InstanceBufferBegin;
		if (currentInstances >= MaxCubes)
		{
			Flush();
			m_InstanceBufferItr = m_InstanceBufferBegin;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		if (rotation.x != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		if (rotation.y != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		if (rotation.z != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		float texIndex = 0.0f;

		m_InstanceBufferItr->Transform = transform;
		m_InstanceBufferItr->Color = color;
		m_InstanceBufferItr->TexIndex = texIndex;

		m_InstanceBufferItr++;
	}

	void CubePass::DrawCube(const glm::vec3& position, const glm::vec3& size, Reference<Texture2D> texture, const glm::vec3& rotation)
	{
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		uint32_t currentInstances = m_InstanceBufferItr - m_InstanceBufferBegin;
		if (currentInstances >= MaxCubes)
		{
			Flush();
			m_InstanceBufferItr = m_InstanceBufferBegin;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		if (rotation.x != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		if (rotation.y != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		if (rotation.z != 0.0f) transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		float texIndex = 0.0f;
		for (uint32_t i = 1; i < m_TextureSlotIndex; i++) {
			if (*m_TextureSlots[i].get() == *texture.get()) {
				texIndex = static_cast<float>(i);
				break;
			}
		}

		if (texIndex == 0.0f) {
			if (m_TextureSlotIndex >= MaxTextureSlots)
			{
				Flush();
				m_InstanceBufferItr = m_InstanceBufferBegin;
				m_TextureSlotIndex = 1;
			}

			texIndex = static_cast<float>(m_TextureSlotIndex);
			m_TextureSlots[m_TextureSlotIndex] = texture;
			m_TextureSlotIndex++;
		}

		m_InstanceBufferItr->Transform = transform;
		m_InstanceBufferItr->Color = color;
		m_InstanceBufferItr->TexIndex = texIndex;

		m_InstanceBufferItr++;
	}

	void CubePass::Flush()
	{
		uint32_t instanceCount = m_InstanceBufferItr - m_InstanceBufferBegin;
		if (instanceCount == 0)
			return;

		uint32_t dataSize = instanceCount * sizeof(CubeInstanceData);
		m_InstanceVertexBuffer->SetData(m_InstanceBufferBegin, dataSize);

		m_TextureShader->Bind();

		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
			m_TextureSlots[i]->Bind(i);

		m_CubeVA->Bind();
		RenderCommand::DrawIndexedInstanced(m_CubeVA, 36, instanceCount);
	}

}