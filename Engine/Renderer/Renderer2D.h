#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Monsi {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene2D(const OrthographicCamera& camera);
		static void EndScene2D();
		static void Flush();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color); //glm::vec3 for z indexing

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color, float scale);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color, float scale);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color, float scale);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, const glm::vec4& color, float scale);

		static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation);
		static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation);

		static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, float rotation);
		static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, float rotation);

		static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, float rotation);
		static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const Reference<SubTexture2D>& subTexture, float rotation);
		
		struct BatchStatistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetVertexCount() const { return QuadCount * 4; }
			uint32_t GetIndexCount() const { return QuadCount * 6; }
		};

		static void ResetBatchStatistics();
		static BatchStatistics GetBatchStatistics();


	private:

		static void FlushResetBatch();
	};

}