#include "MonsiPch.h"
#include "SkyBoxPass.h"
#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>

namespace Monsi {

	SkyBoxPass::SkyBoxPass(const std::array<std::string, 6>& paths)
	{
		m_FilePath = paths;
	}

	void SkyBoxPass::Init()
	{
		m_SkyboxVA = VertexArray::Create();

		float skyboxVertices[24] = {
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f
		};

		uint32_t skyboxIndices[36] = {
			0, 1, 2,  2, 3, 0,
			5, 4, 7,  7, 6, 5,
			4, 0, 3,  3, 7, 4,
			1, 5, 6,  6, 2, 1,
			3, 2, 6,  6, 7, 3,
			4, 5, 1,  1, 0, 4
		};

		m_SkyboxVertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
		m_SkyboxVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPos" }
			});
		m_SkyboxVA->AddVertexBuffer(m_SkyboxVertexBuffer);

		m_SkyboxIB = IndexBuffer::Create(skyboxIndices, 36);
		m_SkyboxVA->SetIndexBuffer(m_SkyboxIB);

		m_SkyboxShader = Shader::Create(SHADER_PATH "SkyboxShader.glsl");

		m_SkyboxShader->Bind();
		m_SkyboxShader->setInt("u_Skybox", 0);
	}

	void SkyBoxPass::Shutdown()
	{
	}

	void SkyBoxPass::DrawSkybox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture)
	{
		glDisable(GL_CULL_FACE); //there is probably a better way, this is a quick solution
		glm::mat4 staticView = glm::mat4(glm::mat3(view));
		glm::mat4 viewProj = projection * staticView;

		RenderCommand::SetDepthFunc(DepthComp::LEQUAL);

		m_SkyboxShader->Bind();
		m_SkyboxShader->setMat4("u_ViewProjection", viewProj);

		skyboxTexture->Bind(0);

		m_SkyboxVA->Bind();
		RenderCommand::DrawIndexed(m_SkyboxVA, 36);

		RenderCommand::SetDepthFunc(DepthComp::LESS);
		glEnable(GL_CULL_FACE);
	}

}
