#include "MonsiPch.h"
#include "SSAOPass.h"
#include "RenderCommand.h"
#include <glad/glad.h>

namespace Monsi {

	void SSAOPass::Init()
	{
		m_SSAOShader = Shader::Create(SHADER_PATH "SSAO.glsl");
		m_SSAOBlurShader = Shader::Create(SHADER_PATH "SSAOBlur.glsl");

		float quadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
		};

		m_QuadVAO = VertexArray::Create();
		m_QuadVBO = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		m_QuadVBO->SetLayout({ { ShaderDataType::Float2, "a_Position" },{ ShaderDataType::Float2, "a_TexCoord" } });
		m_QuadVAO->AddVertexBuffer(m_QuadVBO);

		m_SSAOShader->Bind();
		m_SSAOShader->setInt("gPosition", PositionTextureSlot);
		m_SSAOShader->setInt("gNormal", NormalTextureSlot);
		m_SSAOShader->setInt("texNoise", NoiseTextureSlot);

		m_SSAOBlurShader->Bind();
		m_SSAOBlurShader->setInt("u_SSAOInput", SSAOTextureSlot);
	}

	void SSAOPass::Shutdown()
	{
		m_QuadVAO.reset();
		m_QuadVBO.reset();
		m_SSAOShader.reset();
		m_SSAOBlurShader.reset();
	}

	void SSAOPass::DrawSSAO(const Reference<SSAO>& ssaoInstance, const Reference<FrameBuffer>& gBuffer, const glm::mat4& projection)
	{
		if (!ssaoInstance || !gBuffer) {
			return;
		}

		m_Stats = {};

		const SSAOSettings& settings = ssaoInstance->GetSettings();

		GLint previousFramebuffer = 0;
		GLint previousViewport[4] = { 0, 0, 0, 0 };
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);
		glGetIntegerv(GL_VIEWPORT, previousViewport);

		ssaoInstance->BindSSAOFramebuffer();
		RenderCommand::Clear();

		m_SSAOShader->Bind();
		m_SSAOShader->setMat4("u_Projection", projection);
		m_SSAOShader->setFloat("u_Radius", settings.Radius);
		m_SSAOShader->setFloat("u_Bias", settings.Bias);
		m_SSAOShader->setInt("u_KernelSize", settings.KernelSize);
		m_SSAOShader->setVec2("u_NoiseScale", glm::vec2(settings.Width / 4.0f, settings.Height / 4.0f));

		const auto& kernel = ssaoInstance->GetKernel();
		for (size_t i = 0; i < kernel.size(); ++i) {
			m_SSAOShader->setVec3("u_Samples[" + std::to_string(i) + "]", kernel[i]);
		}

		glBindTextureUnit(PositionTextureSlot, gBuffer->GetColorTextureID());
		glBindTextureUnit(NormalTextureSlot, gBuffer->GetNormalTextureID());
		glBindTextureUnit(NoiseTextureSlot, ssaoInstance->GetNoiseTextureID());

		RenderScreenQuad();

		ssaoInstance->BindBlurFramebuffer();
		RenderCommand::Clear();

		m_SSAOBlurShader->Bind();
		glBindTextureUnit(SSAOTextureSlot, ssaoInstance->GetSSAOTextureID());

		RenderScreenQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)previousFramebuffer);
		glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
	}

	void SSAOPass::Resize(uint32_t width, uint32_t height, const Reference<SSAO>& ssaoInstance)
	{
		ENGINE_PROFILER_FUNCTION();
		if (ssaoInstance) {
			ssaoInstance->Resize(width, height);
		}
	}

	void SSAOPass::RenderScreenQuad()
	{
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		m_Stats.DrawCalls++;
		m_Stats.Triangles += 2;
	}

}