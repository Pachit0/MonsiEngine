#include "MonsiPch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

namespace Monsi {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") { return GL_VERTEX_SHADER; }
		if (type == "fragment" || type == "pixel") { return GL_FRAGMENT_SHADER; }
		ENGINE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) {
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//get name from file
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {

		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());

		for (auto& keyValue : shaderSources) {
			GLenum type = keyValue.first;
			const std::string& source = keyValue.second;
			GLuint shader = glCreateShader(type);

			const GLchar* cStyleSource = source.c_str();
			glShaderSource(shader, 1, &cStyleSource, 0);
			glCompileShader(shader);
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infolog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infolog[0]);

				glDeleteShader(shader);

				ENGINE_LOG_ERROR("{0}", infolog.data());
				ENGINE_ASSERT(false, "Shader compilation failed!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infolog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infolog[0]);

			glDeleteProgram(program);

			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			ENGINE_LOG_ERROR("{0}", infolog.data());
			ENGINE_ASSERT(false, "Shader link failed!");
		}

		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}

		m_ID = program;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* tokenShaderType = "#shadertype";
		size_t tokenTypeLength = strlen(tokenShaderType);
		size_t tokenPosition = source.find(tokenShaderType, 0);
		while (tokenPosition != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", tokenPosition);
			ENGINE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = tokenPosition + tokenTypeLength + 1;
			std::string type = source.substr(begin, eol - begin);
			ENGINE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type used!");

			size_t nextLinePosition = source.find_first_not_of("\r\n", eol);
			tokenPosition = source.find(tokenShaderType, nextLinePosition);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePosition,
				tokenPosition - (nextLinePosition == std::string::npos ? source.size() - 1 : nextLinePosition));
		}

		return shaderSources;
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath) {
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			ENGINE_LOG_ERROR("File couldn't open {0}", filePath);
		}

		return result;
	}

	const void OpenGLShader::setInt(const std::string& name, int value) {
		UploadInt(name, value);
	}

	const void OpenGLShader::setFloat(const std::string& name, float value) {
		UploadFloat(name, value);
	}

	const void OpenGLShader::setVec3(const std::string& name, const glm::vec3& value) {
		UploadVec3(name, value);
	}
	const void OpenGLShader::setVec4(const std::string& name, const glm::vec4& value) {
		UploadVec4(name, value);
	}
	const void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value) {
		UploadMat4(name, value);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadBool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
	}

	void OpenGLShader::UploadInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void OpenGLShader::UploadFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void OpenGLShader::UploadVec2(const std::string& name, const glm::vec2& value) {
		glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}

	void OpenGLShader::UploadVec2(const std::string& name, float x, float y) {
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
	}

	void OpenGLShader::UploadVec3(const std::string& name, const glm::vec3& value) {
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}

	void OpenGLShader::UploadVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
	}

	void OpenGLShader::UploadVec4(const std::string& name, const glm::vec4& value) {
		glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}

	void OpenGLShader::UploadVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
	}

	void OpenGLShader::UploadMat2(const std::string& name, const glm::mat2& mat) {
		glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void OpenGLShader::UploadMat3(const std::string& name, const glm::mat3& mat) {
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void OpenGLShader::UploadMat4(const std::string& name, const glm::mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

}