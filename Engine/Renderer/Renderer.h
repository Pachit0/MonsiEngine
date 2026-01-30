#pragma once

#include "Renderer/RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Monsi {

	class Renderer {
	public:
		static void Begin(OrthographicCamera& camera);
		static void End();
		static void Sumbit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_Scene;
	};

}