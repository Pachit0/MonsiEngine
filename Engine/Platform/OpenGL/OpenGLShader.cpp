#include "MonsiPch.h"
#include "OpenGLShader.h"

namespace Monsi {

	

	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFiles;
		std::ifstream fShaderFiles;

		vShaderFiles.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFiles.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vShaderFiles.open(vertexPath);
			fShaderFiles.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFiles.rdbuf();
			fShaderStream << fShaderFiles.rdbuf();

			vShaderFiles.close();
			fShaderFiles.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			ENGINE_ASSERT(false, "Shader file not found!");
			return;
		}

		// Read our shaders into the appropriate buffers
		std::string vertexSource = vertexCode.c_str();// Get source code for vertex shader.
		std::string fragmentSource = fragmentCode.c_str();// Get source code for fragment shader.

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			ENGINE_LOG_INFO("{0}", infoLog.data());
			ENGINE_ASSERT(false, "VertexShader compilation Error!");

			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			ENGINE_LOG_INFO("{0}", infoLog.data());
			ENGINE_ASSERT(false, "FragmentShader compilation Error!");

			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_ID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);

		// Link our program
		glLinkProgram(m_ID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_ID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			ENGINE_LOG_INFO("{0}", infoLog.data());
			ENGINE_ASSERT(false, "Shader link Error!");

			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_ID, vertexShader);
		glDetachShader(m_ID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::setBool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
	}

	void OpenGLShader::setInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void OpenGLShader::setFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void OpenGLShader::setVec2(const std::string& name, const glm::vec2& value) {
		glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}

	void OpenGLShader::setVec2(const std::string& name, float x, float y) {
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
	}

	void OpenGLShader::setVec3(const std::string& name, const glm::vec3& value) {
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}

	void OpenGLShader::setVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
	}

	void OpenGLShader::setVec4(const std::string& name, const glm::vec4& value) {
		glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}

	void OpenGLShader::setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
	}

	void OpenGLShader::setMat2(const std::string& name, const glm::mat2& mat) {
		glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::setMat3(const std::string& name, const glm::mat3& mat) {
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

}