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

		const void setInt(const std::string& name, int value) override;
		const void setVec3(const std::string& name, const glm::vec3& value) override;
		const void setVec4(const std::string& name, const glm::vec4& value) override;
		const void setMat4(const std::string& name, const glm::mat4& value) override;
		const void setFloat(const std::string& name, float value) override;

		void UploadBool(const std::string& name, bool value);
		void UploadInt(const std::string& name, int value);
		void UploadFloat(const std::string& name, float value);
		void UploadVec2(const std::string& name, const glm::vec2& value);
		void UploadVec2(const std::string& name, float x, float y);
		void UploadVec3(const std::string& name, const glm::vec3& value);
		void UploadVec3(const std::string& name, float x, float y, float z);
		void UploadVec4(const std::string& name, const glm::vec4& value);
		void UploadVec4(const std::string& name, float x, float y, float z, float w);
		void UploadMat2(const std::string& name, const glm::mat2& mat);
		void UploadMat3(const std::string& name, const glm::mat3& mat);
		void UploadMat4(const std::string& name, const glm::mat4& mat);
	private:
		uint32_t m_ID;
		std::string m_Name;

		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	};

}