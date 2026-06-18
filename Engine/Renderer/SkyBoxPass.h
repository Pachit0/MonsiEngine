#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class SkyBoxPass {

	public:
		void Init();
		void Shutdown();
		void DrawSkybox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture);
	
	private:

		Reference<VertexArray> m_SkyboxVA;
		Reference<VertexBuffer> m_SkyboxVertexBuffer;
		Reference<IndexBuffer> m_SkyboxIB;
		Reference<Shader> m_SkyboxShader;

	};

}