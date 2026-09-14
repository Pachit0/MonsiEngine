#include "MonsiPch.h"
#include "Mesh.h"

namespace Monsi {

	uint64_t StaticMesh::s_NextId = 1;
	uint64_t AnimatedMesh::s_NextId = 1;

	StaticMesh::StaticMesh(const std::vector<StaticVertex>& vertices, const std::vector<unsigned int>& indices, const Reference<Material>& material)
		: m_Vertices(vertices), m_Indices(indices), m_Material(material)
	{
		setupMesh();
	}

	void StaticMesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(StaticVertex));

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size()));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	AnimatedMesh::AnimatedMesh(const std::vector<AnimatedVertex>& vertices, const std::vector<unsigned int>& indices, const Reference<Material>& material)
		: m_Vertices(vertices), m_Indices(indices), m_Material(material)
	{
		setupMesh();
	}

	void AnimatedMesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(AnimatedVertex));

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Int4,   "a_BoneIDs" },
			{ ShaderDataType::Float4, "a_Weights" }
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size()));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

}