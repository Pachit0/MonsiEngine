#include "MonsiPch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Monsi {

    struct Quad
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
    };

    struct Renderer3DData
    {
        static const uint32_t MaxQuads = 15000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

		Reference<VertexArray> QuadVA;
		Reference<VertexBuffer> QuadVB;
		Reference<Shader> TextureShader;
		Reference<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		Quad* QuadVertexBufferBegin = nullptr;
		Quad* QuadVertexBufferItr = nullptr;

		std::array<Reference<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec3 QuadVertexPositions[4];
		glm::vec2 QuadTextureCoords[4];
    };

    static Renderer3DData s_Data;

    void Renderer3D::Init()
    {
        s_Data.QuadVA = VertexArray::Create();

        s_Data.QuadVB = VertexBuffer::Create(Renderer3DData::MaxVertices * sizeof(Quad));

        s_Data.QuadVB->SetLayout({
            { ShaderDataType::Float3,"aPos"},
            { ShaderDataType::Float4,"aColor"},
            { ShaderDataType::Float2,"aTexCoord"},
            { ShaderDataType::Float,"aTexIndex"}
            });

        s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

        uint32_t* indices = new uint32_t[Renderer3DData::MaxIndices];

        uint32_t offset = 0;

        for (uint32_t i = 0; i < Renderer3DData::MaxIndices; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        Reference<IndexBuffer> ib = IndexBuffer::Create(indices, Renderer3DData::MaxIndices);
        s_Data.QuadVA->SetIndexBuffer(ib);

        delete[] indices;

        s_Data.QuadVertexBufferBegin = new Quad[Renderer3DData::MaxVertices];

        s_Data.WhiteTexture = Texture2D::Create(1, 1);

        uint32_t white = 0xffffffff;
        s_Data.WhiteTexture->modifyData(&white, sizeof(uint32_t));

        int samplers[Renderer3DData::MaxTextureSlots];

        for (int i = 0; i < Renderer3DData::MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create( SHADER_PATH "Textures3D.glsl");

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->setIntArray("u_Textures", samplers, Renderer3DData::MaxTextureSlots);

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f };

		s_Data.QuadTextureCoords[0] = { 0.0f,0.0f };
		s_Data.QuadTextureCoords[1] = { 1.0f,0.0f };
		s_Data.QuadTextureCoords[2] = { 1.0f,1.0f };
		s_Data.QuadTextureCoords[3] = { 0.0f,1.0f };
    }

    void Renderer3D::Shutdown()
    {
    }

    void Renderer3D::Begin3D(const PerspectiveControl& camera)
    {
        s_Data.TextureShader->Bind();

        s_Data.TextureShader->setMat4("view", camera.GetCamera().GetViewMatrix());
        s_Data.TextureShader->setMat4("projection", camera.GetCamera().GetProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferItr = s_Data.QuadVertexBufferBegin;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer3D::End3D()
    {
        Flush();
    }

    void Renderer3D::Flush()
    {
        if (s_Data.QuadIndexCount == 0)
            return;

        uint32_t size = (uint8_t*)s_Data.QuadVertexBufferItr - (uint8_t*)s_Data.QuadVertexBufferBegin;

        s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBegin, size);

        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
    }

	void Renderer3D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		const float WhiteIndex = 0.0f;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		for (int i = 0; i < 4; i++)
		{
			glm::vec4 transformed = model * glm::vec4(s_Data.QuadVertexPositions[i], 1.0f);
			s_Data.QuadVertexBufferItr->Position = glm::vec3(transformed);
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr->TexIndex = WhiteIndex;
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer3D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, Reference<Texture2D> texture, const glm::vec3& rotation)
	{
		
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				texIndex = static_cast<float>(i);
				break;
			}
		}

		if (texIndex == 0.0f) {
			texIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		for (int i = 0; i < 4; i++)
		{
			glm::vec4 transformed = model * glm::vec4(s_Data.QuadVertexPositions[i], 1.0f);
			s_Data.QuadVertexBufferItr->Position = glm::vec3(transformed);
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr->TexIndex = texIndex;
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		glm::vec3 hs = size * 0.5f;

		// front/back
		DrawQuad(position + glm::vec3(0, 0, hs.z), { size.x,size.y }, color, { 0,0,0 });
		DrawQuad(position + glm::vec3(0, 0, -hs.z), { size.x,size.y }, color, { 0,180,0 });

		// left/right
		DrawQuad(position + glm::vec3(-hs.x, 0, 0), { size.z,size.y }, color, { 0,90,0 });
		DrawQuad(position + glm::vec3(hs.x, 0, 0), { size.z,size.y }, color, { 0,-90,0 });

		// top/bottom
		DrawQuad(position + glm::vec3(0, hs.y, 0), { size.x,size.z }, color, { -90,0,0 });
		DrawQuad(position + glm::vec3(0, -hs.y, 0), { size.x,size.z }, color, { 90,0,0 });
	}

    void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, Reference<Texture2D> tex)
    {
        glm::vec3 hs = size * 0.5f;

        // front/back
		DrawQuad(position + glm::vec3(0, 0, hs.z), { size.x,size.y }, tex, {0,0,0});
		DrawQuad(position + glm::vec3(0, 0, -hs.z), { size.x,size.y }, tex, { 0,180,0 });

        // left/right
		DrawQuad(position + glm::vec3(-hs.x, 0, 0), { size.z,size.y }, tex, { 0,90,0 });
		DrawQuad(position + glm::vec3(hs.x, 0, 0), { size.z,size.y }, tex, { 0,-90,0 });

        // top/bottom
		DrawQuad(position + glm::vec3(0, hs.y, 0), { size.x,size.z }, tex, { -90,0,0 });
		DrawQuad(position + glm::vec3(0, -hs.y, 0), { size.x,size.z }, tex, { 90,0,0 });
    }

}