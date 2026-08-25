#include "MonsiPch.h"
#include "Mesh.h"

#include <glad/glad.h>

namespace Monsi {

	uint64_t Mesh::s_NextId = 1;

	Mesh::Mesh(const std::vector<Vertex_t>& vertices, const std::vector<unsigned int>& indices, const Reference<Material>& material)
		: m_Vertices(vertices), m_Indices(indices), m_Material(material)
	{
		setupMesh();
	}

	void Mesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex_t));

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

}