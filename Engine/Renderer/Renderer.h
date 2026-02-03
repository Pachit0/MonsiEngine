#pragma once

#include "Renderer/RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Monsi {

	class Renderer {
	public:
		static void Init();
		static void onWindowResize(uint32_t width, uint32_t height);

		static void Begin(OrthographicCamera& camera);
		static void End();
		static void Sumbit(const Reference<VertexArray>& vertexArray, const Reference<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_Scene;
	};

}