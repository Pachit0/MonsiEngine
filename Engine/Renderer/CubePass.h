#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Lighting.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>

namespace Monsi {

	struct CubeInstanceData
	{
		glm::mat4 Transform;
		glm::vec4 Color;
		float TexIndex;
	};

	class CubePass {
	public:
		void Init();
		void Shutdown();

		void BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting);
		void EndScene();

		void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);
		void DrawCube(const glm::vec3& position, const glm::vec3& size, const Reference<Texture2D>& texture, const glm::vec3& rotation);

	private:
		void Flush();

	private:
		static constexpr uint32_t MaxCubes = 10000;
		static constexpr uint32_t MaxTextureSlots = 32;

		Reference<VertexArray> m_CubeVA;
		Reference<VertexBuffer> m_CubeVertexBuffer;
		Reference<VertexBuffer> m_InstanceVertexBuffer;
		Reference<IndexBuffer> m_CubeIB;

		Reference<Shader> m_TextureShader;
		Reference<Texture2D> m_WhiteTexture;

		CubeInstanceData* m_InstanceBufferBegin = nullptr;
		CubeInstanceData* m_InstanceBufferItr = nullptr;

		std::array<Reference<Texture2D>, MaxTextureSlots> m_TextureSlots;
		uint32_t m_TextureSlotIndex = 0;

		glm::mat4 m_ViewProjection;
	};

}