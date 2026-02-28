#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Monsi {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		
		static void Begin2D(const OrthographicCamera& camera);
		static void End2D();
		static void Flush();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color); //glm::vec3 for z indexing

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color, float scale);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, const glm::vec4& color, float scale);
		//TODO
		//Rotated quad, also the glsl file should handle that -- goodluck future Pachi

		static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation);
		static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation);

		static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture, float rotation);
		static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture, float rotation);
	};

}