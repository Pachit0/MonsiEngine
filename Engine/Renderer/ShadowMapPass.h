#pragma once

#include "StaticModel.h"
#include "AnimatedModel.h"
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

		void SubmitModel(const Reference<StaticModel>& model, const glm::mat4& transform);
		void SubmitMesh(const StaticMesh* meshPtr, const glm::mat4& transform);
		void SubmitAnimatedModel(const Reference<AnimatedModel>& model, const glm::mat4& transform);

		void ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap);

		void DrawShadowMap(const glm::mat4& lightViewProjection, const Reference<ShadowMap>& shadowMap);

		void Clear();

		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t Triangles = 0;
		};
		const Stats& GetStats() const { return m_Stats; }
		void ResetStats() { m_Stats = {}; }

	private:
		void DrawMeshDepthOnly(const StaticMesh* meshPtr, const glm::mat4& transform);
		void DrawAnimatedMeshDepthOnly(const AnimatedMesh* meshPtr, const AnimatedModel* modelPtr, const glm::mat4& transform);

		struct StaticShadowCasterCommand {
			const StaticMesh* MeshPtr = nullptr;
			Reference<StaticModel> ModelRef = nullptr;
			glm::mat4 Transform{ 1.0f };
		};

		struct AnimatedShadowCasterCommand {
			Reference<AnimatedModel> ModelRef = nullptr;
			glm::mat4 Transform{ 1.0f };
		};

		std::vector<StaticShadowCasterCommand> m_StaticCommands;
		std::vector<AnimatedShadowCasterCommand> m_AnimatedCommands;

		Reference<Shader> m_ShadowMapShader;
		Reference<Shader> m_AnimatedShadowMapShader;

		Stats m_Stats;
	};

}