#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Monsi {

	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual const void setInt(const std::string& name, int value) = 0;
		virtual const void setIntArray(const std::string& name, int* value, uint32_t count) = 0;
		virtual const void setFloat(const std::string& name, float value) = 0;
		virtual const void setVec3(const std::string& name, const glm::vec3& value) = 0;
		virtual const void setVec4(const std::string& name, const glm::vec4& value) = 0;
		virtual const void setMat4(const std::string& name, const glm::mat4& value) = 0;

		static Reference<Shader> Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		static Reference<Shader> Create(const std::string& filePath);
	};

	class ShaderLibrary {
	public:
		void Add(const std::string& name, const Reference<Shader>& shader);
		void Add(const Reference<Shader>& shader);
		Reference<Shader> Load(const std::string& name, const std::string& filePath);
		Reference<Shader> Load(const std::string& filePath);
		Reference<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	
	private:
		std::unordered_map<std::string, Reference<Shader>> m_ShaderMap;
	};
}