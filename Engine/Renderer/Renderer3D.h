#pragma once

#include "Texture.h"
#include "ModelLoader.h"
#include "Lighting.h"
#include "ShadowMap.h"
#include <glm/glm.hpp>

namespace Monsi {

    class Renderer3D
    {
    public:

        static void Init();
        static void Shutdown();

		static void Begin3D(const glm::mat4& viewProjection, const glm::vec3& cameraPosition);
        static void End3D();

		static void DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color);
		static void DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color);

		static void DrawSkyBox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture);
		static void DrawShadowMap(const glm::mat4& view, const glm::mat4& projection, const Reference<ShadowMap>& shadowMap);

		static void SetSceneLighting(const SceneLighting& lighting);
        static void SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap);
        static void ResizeShadowMap(uint32_t width, uint32_t height, const Reference<ShadowMap>& shadowMap);
    };

}