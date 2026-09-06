#pragma once

#include "ModelLoader.h"
#include "ShadowMap.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include <glm/glm.hpp>
#include <vector>

namespace Monsi {

	class ShadowMapPass {
	public:
		void Init();
		void Shutdown();

		void SubmitModel(const Reference<Model>& model, const glm::mat4& transform);
		void SubmitMesh(const Mesh* meshPtr, const glm::mat4& transform);

		void ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap);

		void DrawShadowMap(const glm::mat4& lightViewProjection, const Reference<ShadowMap>& shadowMap);

		void Clear();

	private:
		void DrawMeshDepthOnly(const Mesh* meshPtr, const glm::mat4& transform);

		struct ShadowCasterCommand {
			const Mesh* MeshPtr = nullptr;
			Reference<Model> ModelRef = nullptr;
			glm::mat4 Transform{ 1.0f };
		};

		std::vector<ShadowCasterCommand> m_Commands;
		Reference<Shader> m_ShadowMapShader;
	};

}