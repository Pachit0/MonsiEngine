#include "MonsiPch.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Monsi {

	Reference<Shader> Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexPath, fragmentPath);
		}
	ENGINE_ASSERT(false, "Unknown Shader!");
	return nullptr;
	}

	Reference<Shader> Shader::Create(const std::string& filePath) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filePath);
		}
		ENGINE_ASSERT(false, "Unknown Shader!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Reference<Shader>& shader) {
		ENGINE_ASSERT(!Exists(name), "Shader already exists!");
		m_ShaderMap[name] = shader;
	}

	void ShaderLibrary::Add(const Reference<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Monsi::Reference<Monsi::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Monsi::Reference<Monsi::Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Monsi::Reference<Monsi::Shader> ShaderLibrary::Get(const std::string& name)
	{
		ENGINE_ASSERT(Exists(name), "Shader not found!");
		return m_ShaderMap[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_ShaderMap.find(name) != m_ShaderMap.end();
	}

}