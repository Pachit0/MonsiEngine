#include "MonsiPch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Monsi {

	struct Renderer2DData {
		Reference<VertexArray> QuadVA;
		Reference<Shader> FlatColorShader;
		Reference<Shader> TextureShader;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DData();
		s_Data->QuadVA = VertexArray::Create();

		float squareVertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Reference<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float2, "TexCoord"}

		};
		squareVB->SetLayout(squareLayout);
		s_Data->QuadVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 0,1,2,2,3,0 };

		Reference<IndexBuffer> squareEB;
		squareEB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVA->SetIndexBuffer(squareEB);

		s_Data->FlatColorShader = Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/ShaderFlatColor.glsl");
		s_Data->TextureShader = Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/Textures.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->setInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::Begin2D(const OrthographicCamera& camera) {
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->setMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->setMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
	}

	void Renderer2D::End2D() {

	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->setVec4("u_Color", color);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) /* Translation * rotation(optinal) * scale */
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->FlatColorShader->setMat4("u_Transform", transform);

		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Reference<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Reference<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->setMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const Reference<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Reference<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->setVec4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->setMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const Reference<Texture2D>& texture, float scale)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color, texture, scale);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Reference<Texture2D>& texture, float scale)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->setVec4("u_Color", color);
		s_Data->TextureShader->setFloat("u_Scale", scale);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->setMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
	}


}