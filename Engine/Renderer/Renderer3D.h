#pragma once

#include "PerspectiveControl.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

    class Renderer3D
    {
    public:

        static void Init();
        static void Shutdown();

        static void Begin3D(const PerspectiveControl& camera);
        static void End3D();

        static void Flush();

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec3& rotation);
        static void DrawQuad(const glm::vec3& position,const glm::vec2& size,Reference<Texture2D> texture,const glm::vec3& rotation);
        
        static void DrawCube(const glm::vec3& position,const glm::vec3& size,const glm::vec4& color);
        static void DrawCube(const glm::vec3& position,const glm::vec3& size,Reference<Texture2D> texture);
    };

}