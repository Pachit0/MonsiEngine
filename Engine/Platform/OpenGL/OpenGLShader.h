#pragma once

#include "Renderer/Shader.h"

namespace Monsi {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;


		void setBool(const std::string& name, bool value) override;
		void setInt(const std::string& name, int value) override;
		void setFloat(const std::string& name, float value) override;
		void setVec2(const std::string& name, const glm::vec2& value) override;
		void setVec2(const std::string& name, float x, float y) override;
		void setVec3(const std::string& name, const glm::vec3& value) override;
		void setVec3(const std::string& name, float x, float y, float z) override;
		void setVec4(const std::string& name, const glm::vec4& value) override;
		void setVec4(const std::string& name, float x, float y, float z, float w) override;
		void setMat2(const std::string& name, const glm::mat2& mat) override;
		void setMat3(const std::string& name, const glm::mat3& mat) override;
		void setMat4(const std::string& name, const glm::mat4& mat) override;
	private:
		uint32_t m_ID;
	};

}