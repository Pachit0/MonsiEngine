#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class SkyBoxPass {

	public:
		SkyBoxPass() = default;
		SkyBoxPass(const std::array<std::string, 6>& paths);

		void Init();
		void Shutdown();
		void DrawSkybox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture);
		const std::array<std::string, 6>& GetFilePath() const { return m_FilePath; }
	private:

		Reference<VertexArray> m_SkyboxVA;
		Reference<VertexBuffer> m_SkyboxVertexBuffer;
		Reference<IndexBuffer> m_SkyboxIB;
		Reference<Shader> m_SkyboxShader;

		std::array<std::string, 6> m_FilePath;
	};

}