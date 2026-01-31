#pragma once

#include "Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Monsi {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name ,const std::string& vertexPath, const std::string& fragmentPath);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		const std::string& GetName() const override { return m_Name; }

		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setVec2(const std::string& name, const glm::vec2& value);
		void setVec2(const std::string& name, float x, float y);
		void setVec3(const std::string& name, const glm::vec3& value);
		void setVec3(const std::string& name, float x, float y, float z);
		void setVec4(const std::string& name, const glm::vec4& value);
		void setVec4(const std::string& name, float x, float y, float z, float w);
		void setMat2(const std::string& name, const glm::mat2& mat);
		void setMat3(const std::string& name, const glm::mat3& mat);
		void setMat4(const std::string& name, const glm::mat4& mat);
	private:
		uint32_t m_ID;
		std::string m_Name;

		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	};

}