#pragma once

#include <vector>
#include <string>

#include "VertexArray.h"
#include "Shader.h"
#include "Material.h"

#include <glm/glm.hpp>

namespace Monsi {

	struct Vertex_t
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex_t>& vertices, const std::vector<unsigned int>& indices, const Reference<Material>& material);

		const Reference<Material>& GetMaterial() const { return m_Material; }
		void SetMaterial(const Reference<Material>& material) { m_Material = material; }

		const Reference<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return m_Indices.size(); }

	private:
		void setupMesh();

	private:
		std::vector<Vertex_t> m_Vertices;
		std::vector<unsigned int> m_Indices;
		Reference<Material> m_Material;

		Reference<VertexArray> m_VertexArray;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;
	};

}