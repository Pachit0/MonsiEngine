#pragma once

#include <vector>
#include <string>

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

namespace Monsi {

	struct Vertex_t
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture_t
	{
		Reference<Texture2D> texture;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex_t>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture_t>& textures);

		const std::vector<Texture_t>& GetTextures() const { return p_textures; }
		const Reference<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return p_indices.size(); }

	private:
		void setupMesh();

	private:
		std::vector<Vertex_t> p_vertices;
		std::vector<unsigned int> p_indices;
		std::vector<Texture_t> p_textures;

		Reference<VertexArray> m_VertexArray;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;
	};

}