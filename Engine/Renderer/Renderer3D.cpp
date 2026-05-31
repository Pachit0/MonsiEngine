#include "MonsiPch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"


namespace Monsi {

	struct CubeData {
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	struct InstanceData {
		glm::mat4 Transform;
		glm::vec4 Color;
		float TexIndex;
	};

	struct Renderer3DData {
		static const uint32_t MaxCubes = 15000;
		static const uint32_t MaxTextureSlots = 32;

		Reference<VertexArray> CubeVA;
		Reference<VertexBuffer> CubeVertexBuffer;
		Reference<VertexBuffer> InstanceVertexBuffer;
		Reference<IndexBuffer> CubeIB;

		Reference<Shader> TextureShader;
		Reference<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		InstanceData* InstanceBufferBegin = nullptr;
		InstanceData* InstanceBufferItr = nullptr;

		glm::vec3 CubeVertexPositions[24];
		glm::vec2 CubeTextureCoords[24];

		std::array<Reference<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.CubeVA = VertexArray::Create();

		s_Data.CubeVertexPositions[0] = { -0.5f, -0.5f,  0.5f };
		s_Data.CubeVertexPositions[1] = { 0.5f, -0.5f,  0.5f };
		s_Data.CubeVertexPositions[2] = { 0.5f,  0.5f,  0.5f };
		s_Data.CubeVertexPositions[3] = { -0.5f,  0.5f,  0.5f };

		s_Data.CubeVertexPositions[4] = { 0.5f, -0.5f,  0.5f };
		s_Data.CubeVertexPositions[5] = { 0.5f, -0.5f, -0.5f };
		s_Data.CubeVertexPositions[6] = { 0.5f,  0.5f, -0.5f };
		s_Data.CubeVertexPositions[7] = { 0.5f,  0.5f,  0.5f };

		s_Data.CubeVertexPositions[8] = { 0.5f, -0.5f, -0.5f };
		s_Data.CubeVertexPositions[9] = { -0.5f, -0.5f, -0.5f };
		s_Data.CubeVertexPositions[10] = { -0.5f,  0.5f, -0.5f };
		s_Data.CubeVertexPositions[11] = { 0.5f,  0.5f, -0.5f };

		s_Data.CubeVertexPositions[12] = { -0.5f, -0.5f, -0.5f };
		s_Data.CubeVertexPositions[13] = { -0.5f, -0.5f,  0.5f };
		s_Data.CubeVertexPositions[14] = { -0.5f,  0.5f,  0.5f };
		s_Data.CubeVertexPositions[15] = { -0.5f,  0.5f, -0.5f };

		s_Data.CubeVertexPositions[16] = { -0.5f,  0.5f,  0.5f };
		s_Data.CubeVertexPositions[17] = { 0.5f,  0.5f,  0.5f };
		s_Data.CubeVertexPositions[18] = { 0.5f,  0.5f, -0.5f };
		s_Data.CubeVertexPositions[19] = { -0.5f,  0.5f, -0.5f };

		s_Data.CubeVertexPositions[20] = { -0.5f, -0.5f, -0.5f };
		s_Data.CubeVertexPositions[21] = { 0.5f, -0.5f, -0.5f };
		s_Data.CubeVertexPositions[22] = { 0.5f, -0.5f,  0.5f };
		s_Data.CubeVertexPositions[23] = { -0.5f, -0.5f,  0.5f };

		for (int i = 0; i < 24; i += 4)
		{
			s_Data.CubeTextureCoords[i + 0] = { 0.0f, 0.0f };
			s_Data.CubeTextureCoords[i + 1] = { 1.0f, 0.0f };
			s_Data.CubeTextureCoords[i + 2] = { 1.0f, 1.0f };
			s_Data.CubeTextureCoords[i + 3] = { 0.0f, 1.0f };
		}

		CubeData staticVertices[24];
		for (int i = 0; i < 24; i++) {
			staticVertices[i].Position = s_Data.CubeVertexPositions[i];
			staticVertices[i].TexCoord = s_Data.CubeTextureCoords[i];
		}

		uint32_t totalSize = 24 * sizeof(CubeData);

		s_Data.CubeVertexBuffer = VertexBuffer::Create((float*)staticVertices, totalSize);
		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.CubeVA->AddVertexBuffer(s_Data.CubeVertexBuffer);

		s_Data.InstanceVertexBuffer = VertexBuffer::Create(Renderer3DData::MaxCubes * sizeof(InstanceData));
		s_Data.InstanceVertexBuffer->SetLayout({
			{ ShaderDataType::Float4, "a_MtxRow0",  true },
			{ ShaderDataType::Float4, "a_MtxRow1",  true },
			{ ShaderDataType::Float4, "a_MtxRow2",  true },
			{ ShaderDataType::Float4, "a_MtxRow3",  true },
			{ ShaderDataType::Float4, "a_Color",    true },
			{ ShaderDataType::Float,  "a_TexIndex", true }
		});

		s_Data.CubeVA->AddVertexBuffer(s_Data.InstanceVertexBuffer);

		uint32_t indices[36] = {
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		s_Data.CubeIB = IndexBuffer::Create(indices, 36);
		s_Data.CubeVA->SetIndexBuffer(s_Data.CubeIB);

		s_Data.InstanceBufferBegin = new InstanceData[Renderer3DData::MaxCubes];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteData = 0xffffffff;
		s_Data.WhiteTexture->modifyData(&whiteData, sizeof(uint32_t));

		int samplers[Renderer3DData::MaxTextureSlots];
		for (int i = 0; i < Renderer3DData::MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create(SHADER_PATH "Textures3D.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->setIntArray("u_Textures", samplers, Renderer3DData::MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::Begin3D(const PerspectiveControl& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->setMat4("u_ViewProjection", camera.GetCamera().GetViewProjectionMatrix());

		s_Data.InstanceBufferItr = s_Data.InstanceBufferBegin;
		s_Data.CubeIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::Flush()
	{
		uint32_t instanceCount = s_Data.InstanceBufferItr - s_Data.InstanceBufferBegin;

		if (instanceCount == 0)
			return;

		uint32_t dataSize = instanceCount * sizeof(InstanceData);

		s_Data.InstanceVertexBuffer->SetData(s_Data.InstanceBufferBegin, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexedInstanced(s_Data.CubeVA, 0, instanceCount);
	}

	void Renderer3D::End3D()
	{
		Flush();
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		uint32_t currentInstances = s_Data.InstanceBufferItr - s_Data.InstanceBufferBegin;
		if (currentInstances >= Renderer3DData::MaxCubes)
		{
			Flush();
			s_Data.InstanceBufferItr = s_Data.InstanceBufferBegin;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);
		
		float texIndex = 0.0f;

		s_Data.InstanceBufferItr->Transform = transform;
		s_Data.InstanceBufferItr->Color = color;
		s_Data.InstanceBufferItr->TexIndex = texIndex;

		s_Data.InstanceBufferItr++;
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, Reference<Texture2D> texture, const glm::vec3& rotation)
	{
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f ,1.0f };

		uint32_t currentInstances = s_Data.InstanceBufferItr - s_Data.InstanceBufferBegin;

		if (currentInstances >= Renderer3DData::MaxCubes)
		{
			Flush();
			s_Data.InstanceBufferItr = s_Data.InstanceBufferBegin;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, size);

		float texIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				texIndex = static_cast<float>(i);
				break;
			}
		}

		if (texIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer3DData::MaxTextureSlots)
			{
				Flush();
				s_Data.InstanceBufferItr = s_Data.InstanceBufferBegin;
				s_Data.TextureSlotIndex = 1;
			}

			texIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.InstanceBufferItr->Transform = transform;
		s_Data.InstanceBufferItr->Color = color;
		s_Data.InstanceBufferItr->TexIndex = texIndex;

		s_Data.InstanceBufferItr++;
	}

}