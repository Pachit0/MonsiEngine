#pragma once

#include "Texture.h"
#include "StaticModel.h"
#include "AnimatedModel.h"
#include "Lighting.h"
#include "ShadowMap.h"
#include <glm/glm.hpp>

namespace Monsi {

	struct Renderer3DStats
	{
		uint32_t ModelDrawCalls = 0;
		uint32_t ModelInstances = 0;
		uint32_t ModelTriangles = 0;

		uint32_t AnimatedModelDrawCalls = 0;
		uint32_t AnimatedModelInstances = 0;
		uint32_t AnimatedModelTriangles = 0;

		uint32_t ShadowDrawCalls = 0;
		uint32_t ShadowTriangles = 0;

		uint32_t SkyboxDrawCalls = 0;
		uint32_t SkyboxTriangles = 0;

		uint32_t GetTotalDrawCalls() const { return ModelDrawCalls + AnimatedModelDrawCalls + ShadowDrawCalls + SkyboxDrawCalls; }
		uint32_t GetTotalTriangles() const { return ModelTriangles + AnimatedModelTriangles + ShadowTriangles + SkyboxTriangles; }
	};

	class Renderer3D
	{
	public:

		static void Init();
		static void Shutdown();

		static void Begin3D(const glm::mat4& viewProjection, const glm::vec3& cameraPosition);
		static void End3D();

		static void DrawModel(const Reference<StaticModel>& model, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawMesh(const StaticMesh* meshPtr, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawAnimatedModel(const Reference<AnimatedModel>& model, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawSkyBox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture);
		static void DrawShadowMap(const glm::mat4& view, const glm::mat4& projection, const Reference<ShadowMap>& shadowMap);

		static void SetSceneLighting(const SceneLighting& lighting);
		static void SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap, float shadowIntensity);
		static void ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap);

		static Renderer3DStats GetStats();
		static void ResetStats();
	};

}