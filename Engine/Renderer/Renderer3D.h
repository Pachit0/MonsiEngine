#pragma once

#include "Texture.h"
#include "ModelLoader.h"
#include "Lighting.h"
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

		static void SetSceneLighting(const SceneLighting& lighting);

// 	       Deprecated methods - will be removed
// 	       static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);
// 	       static void DrawCube(const glm::vec3& position,const glm::vec3& size, Reference<Texture2D> texture,const glm::vec3& rotation);
        

//         static void DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
//         static void DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);
// 		   static void DrawMesh(const Mesh* meshPtr, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);
    };

}