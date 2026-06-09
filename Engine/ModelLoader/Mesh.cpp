#include "MonsiPch.h"
#include "Mesh.h"

#include <glad/glad.h>

namespace Monsi {

	Mesh::Mesh(const std::vector<Vertex_t>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture_t>& textures)
		: p_vertices(vertices), p_indices(indices), p_textures(textures)
	{
		setupMesh();
	}

	void Mesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create((float*)p_vertices.data(), p_vertices.size() * sizeof(Vertex_t));

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(p_indices.data(), p_indices.size());

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

}