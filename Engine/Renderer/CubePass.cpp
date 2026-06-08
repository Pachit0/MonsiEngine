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

		CubeVA = VertexArray::Create();

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

		uint32_t indices[36] =
		{
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		CubeVertexBuffer = VertexBuffer::Create((float*)vertices, sizeof(vertices));

		CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		CubeVA->AddVertexBuffer(CubeVertexBuffer);

		InstanceVertexBuffer = VertexBuffer::Create(MaxCubes * sizeof(CubeInstanceData));

		InstanceVertexBuffer->SetLayout({
			{ ShaderDataType::Float4, "a_MtxRow0",  true },
			{ ShaderDataType::Float4, "a_MtxRow1",  true },
			{ ShaderDataType::Float4, "a_MtxRow2",  true },
			{ ShaderDataType::Float4, "a_MtxRow3",  true },
			{ ShaderDataType::Float4, "a_Color",    true },
			{ ShaderDataType::Float,  "a_TexIndex", true }
			});

		CubeVA->AddVertexBuffer(InstanceVertexBuffer);

		CubeIB = IndexBuffer::Create(indices, 36);
		CubeVA->SetIndexBuffer(CubeIB);

		InstanceBufferBegin = new CubeInstanceData[MaxCubes];
		InstanceBufferItr = InstanceBufferBegin;

		WhiteTexture = Texture2D::Create(1, 1);

		uint32_t whitePixel = 0xffffffff;
		WhiteTexture->modifyData(&whitePixel, sizeof(uint32_t));

		TextureSlots[0] = WhiteTexture;
		TextureSlotIndex = 1;

		TextureShader = Shader::Create(SHADER_PATH "Textures3D.glsl");

		int samplers[MaxTextureSlots];

		for (uint32_t i = 0; i < MaxTextureSlots; i++)
			samplers[i] = i;

		TextureShader->Bind();
		TextureShader->setIntArray("u_Textures", samplers, MaxTextureSlots);
	}

	void CubePass::Shutdown()
	{

	}

	void CubePass::BeginScene(const glm::mat4& viewProjection)
	{
		m_ViewProjection = viewProjection;

		TextureShader->Bind();
		TextureShader->setMat4("u_ViewProjection", m_ViewProjection);

		InstanceBufferItr = InstanceBufferBegin;
		TextureSlotIndex = 1;
	}

	void CubePass::EndScene()
	{
		Flush();
	}

	void CubePass::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		uint32_t currentInstances = InstanceBufferItr - InstanceBufferBegin;
		if (currentInstances >= MaxCubes)
		{
			Flush();
			InstanceBufferItr = InstanceBufferBegin;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		float texIndex = 0.0f;

		InstanceBufferItr->Transform = transform;
		InstanceBufferItr->Color = color;
		InstanceBufferItr->TexIndex = texIndex;

		InstanceBufferItr++;
	}

	void CubePass::DrawCube(const glm::vec3& position, const glm::vec3& size, Reference<Texture2D> texture, const glm::vec3& rotation)
	{
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f ,1.0f };

		uint32_t currentInstances = InstanceBufferItr - InstanceBufferBegin;

		if (currentInstances >= MaxCubes)
		{
			Flush();
			InstanceBufferItr = InstanceBufferBegin;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		float texIndex = 0.0f;
		for (uint32_t i = 1; i < TextureSlotIndex; i++) {
			if (*TextureSlots[i].get() == *texture.get()) {
				texIndex = static_cast<float>(i);
				break;
			}
		}

		if (texIndex == 0.0f) {
			if (TextureSlotIndex >= MaxTextureSlots)
			{
				Flush();
				InstanceBufferItr = InstanceBufferBegin;
				TextureSlotIndex = 1;
			}

			texIndex = static_cast<float>(TextureSlotIndex);
			TextureSlots[TextureSlotIndex] = texture;
			TextureSlotIndex++;
		}

		InstanceBufferItr->Transform = transform;
		InstanceBufferItr->Color = color;
		InstanceBufferItr->TexIndex = texIndex;

		InstanceBufferItr++;
	}

	void CubePass::Flush()
	{
		uint32_t instanceCount = InstanceBufferItr - InstanceBufferBegin;

		if (instanceCount == 0)
			return;

		uint32_t dataSize = instanceCount * sizeof(CubeInstanceData);

		InstanceVertexBuffer->SetData(InstanceBufferBegin, dataSize);

		TextureShader->Bind();

		for (uint32_t i = 0; i < TextureSlotIndex; i++)
			TextureSlots[i]->Bind(i);

		CubeVA->Bind();

		RenderCommand::DrawIndexedInstanced(CubeVA, 36, instanceCount);
	}

}