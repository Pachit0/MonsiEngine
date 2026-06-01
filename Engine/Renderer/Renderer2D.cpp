#include "MonsiPch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Monsi {

	struct Quad {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float Scale;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuadsPerDrawCall = 30000;
		static const uint32_t MaxVerticesPerDrawCall = MaxQuadsPerDrawCall * 4;
		static const uint32_t MaxIndicesPerDrawCall = MaxQuadsPerDrawCall * 6;
		static const uint32_t MaxTextureSlots = 32;

		Reference<VertexArray> QuadVA;
		Reference<VertexBuffer> QuadVB;
		Reference<Shader> TextureShader;
		Reference<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		Quad* QuadVertexBufferBegin = nullptr;
		Quad* QuadVertexBufferItr = nullptr;

		glm::vec4 QuadVertexPositions[4];
		glm::vec2 QuadTextureCoords[4];

		std::array<Reference<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;// first slot is reserved for the white texture

		Renderer2D::BatchStatistics batchStats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		ENGINE_PROFILER_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();

		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVerticesPerDrawCall * sizeof(Quad));
		s_Data.QuadVB->SetLayout({
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float4, "aColor"},
			{ ShaderDataType::Float2, "aTexCoord"},
			{ ShaderDataType::Float, "aTexIndex"},
			{ ShaderDataType::Float, "aScale"}
		});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBegin = new Quad[s_Data.MaxVerticesPerDrawCall];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndicesPerDrawCall];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndicesPerDrawCall; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Reference<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndicesPerDrawCall);
		s_Data.QuadVA->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t WhiteColorData = 0xffffffff;
		s_Data.WhiteTexture->modifyData(&WhiteColorData, sizeof(uint32_t));

		int32_t samplers2D[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers2D[i] = i;
		}

		s_Data.TextureShader = Shader::Create( SHADER_PATH "Textures.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->setIntArray("u_Textures", samplers2D, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.QuadTextureCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadTextureCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadTextureCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadTextureCoords[3] = { 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		ENGINE_PROFILER_FUNCTION();
	}

	void Renderer2D::BeginScene2D(const OrthographicCamera& camera) {
		ENGINE_PROFILER_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->setMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadVertexBufferItr = s_Data.QuadVertexBufferBegin;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene2D(const Camera& camera, const glm::mat4& transform)
	{
		ENGINE_PROFILER_FUNCTION();

		glm::mat4 viewProjection = camera.GetProjectionMatrix() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->setMat4("u_ViewProjection", viewProjection);

		s_Data.QuadVertexBufferItr = s_Data.QuadVertexBufferBegin;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene2D() {
		ENGINE_PROFILER_FUNCTION();

		uint32_t sizeOfData = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferItr) - reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBegin);
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBegin, sizeOfData);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
		s_Data.batchStats.DrawCalls++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		const float whiteTexIndex = 0.0f;
		const float scale = 1.0f;

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = whiteTexIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const Reference<Texture2D>& texture, const glm::vec4& color, float scale)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	
	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		ENGINE_PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		const float scale = 1.0f;
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f ,1.0f };
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;

		/*ENGINE_PROFILER_FUNCTION();
		s_Data.TextureShader->setFloat("u_Scale", 1.0f);
		s_Data.TextureShader->setVec4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->setMat4("u_Transform", transform);


		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);*/
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		const float scale = 1.0f;
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color, float scale)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, color, scale);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color, float scale)
	{
		ENGINE_PROFILER_FUNCTION();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, color, scale);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subTexture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		Reference<Texture2D> texture = subTexture->GetTexture();
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();

		const float scale = 1.0f;
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f ,1.0f };
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subTexture, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		Reference<Texture2D> texture = subTexture->GetTexture();
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();

		const float scale = 1.0f;
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color, float scale)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subTexture, color, scale);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color, float scale)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		Reference<Texture2D> texture = subTexture->GetTexture();
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float scale)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color, scale);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float scale)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		const float whiteTexIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = whiteTexIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		drawQuadRotated({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		const float whiteTexIndex = 0.0f;
		const float scale = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = whiteTexIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, float rotation)
	{
		drawQuadRotated({ position.x, position.y, 0.0f }, size, texture, rotation);
	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, float rotation)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		const float scale = 1.0f;
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f ,1.0f };
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = s_Data.QuadTextureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, float rotation)
	{
		drawQuadRotated({ position.x, position.y, 0.0f }, size, subTexture, rotation);
	}

	void Renderer2D::drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, float rotation)
	{
		ENGINE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndicesPerDrawCall) {
			FlushResetBatch();
		}

		Reference<Texture2D> texture = subTexture->GetTexture();
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();

		const float scale = 1.0f;
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f ,1.0f };
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
				FlushResetBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferItr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferItr->Color = color;
			s_Data.QuadVertexBufferItr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferItr->Scale = scale;
			s_Data.QuadVertexBufferItr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferItr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.batchStats.QuadCount++;
	}

	void Renderer2D::ResetBatchStatistics()
	{
		memset(&s_Data.batchStats, 0, sizeof(BatchStatistics));
	}

	Renderer2D::BatchStatistics Renderer2D::GetBatchStatistics()
	{
		return s_Data.batchStats;
	}

	void Renderer2D::FlushResetBatch()
	{
		EndScene2D();

		s_Data.QuadVertexBufferItr = s_Data.QuadVertexBufferBegin;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

}