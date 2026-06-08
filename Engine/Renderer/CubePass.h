#pragma once

#include "VertexArray.h"
#include "Shader.h"
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

		void BeginScene(const glm::mat4& viewProjection);
		void EndScene();

		void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);
		void DrawCube(const glm::vec3& position, const glm::vec3& size, Reference<Texture2D> texture, const glm::vec3& rotation);

	private:
		void Flush();

	private:
		static constexpr uint32_t MaxCubes = 10000;
		static constexpr uint32_t MaxTextureSlots = 32;

		Reference<VertexArray> CubeVA;
		Reference<VertexBuffer> CubeVertexBuffer;
		Reference<VertexBuffer> InstanceVertexBuffer;
		Reference<IndexBuffer> CubeIB;

		Reference<Shader> TextureShader;
		Reference<Texture2D> WhiteTexture;

		CubeInstanceData* InstanceBufferBegin = nullptr;
		CubeInstanceData* InstanceBufferItr = nullptr;

		std::array<Reference<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 0;

		glm::mat4 m_ViewProjection;
	};

}