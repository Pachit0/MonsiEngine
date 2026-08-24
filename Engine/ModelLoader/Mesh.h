#pragma once

#include <vector>
#include <string>

#include "VertexArray.h"
#include "Shader.h"
#include "Material.h"
#include "PrimitiveParams.h"

#include <glm/glm.hpp>

namespace Monsi {

	enum class PrimitiveType {
		None = 0, Sphere, Grid, Cube, Cylinder, Cone, Torus, Quad
	};

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

		const Reference<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return m_Indices.size(); }

		void SetMaterial(const Reference<Material>& material) { m_Material = material; }
		void SetAmbientColor(const glm::vec3& color) { m_Material->AmbientColor = color; }
		void SetDiffuseColor(const glm::vec3& color) { m_Material->DiffuseColor = color; }
		void SetSpecularColor(const glm::vec3& color) { m_Material->SpecularColor = color; }
		void SetShininess(float shine) { m_Material->Shininess = shine; }
		void SetDiffuseMap(const Reference<Texture2D>& tex) { m_Material->DiffuseMap = tex; }
		void SetSpecularMap(const Reference<Texture2D>& tex) { m_Material->SpecularMap = tex; }
		void SetNormalMap(const Reference<Texture2D>& tex) { m_Material->NormalMap = tex; }
		void SetPrimitiveType(const PrimitiveType& type) { m_Type = type; }
		void SetPrimitiveParams(const PrimitiveParams& params) { m_Params = params; }

		const PrimitiveParams& GetPrimitiveParams() const { return m_Params; }
		const PrimitiveType& GetPrimitiveType() const { return m_Type; }
		float GetShininess() const { return m_Material->Shininess; }
		const Reference<Material>& GetMaterial() const { return m_Material; }
		const glm::vec3& GetAmbientColor() const { return m_Material->AmbientColor; }
		const glm::vec3& GetDiffuseColor() const { return m_Material->DiffuseColor; }
		const glm::vec3& GetSpecularColor() const { return m_Material->SpecularColor; }
		const Reference<Texture2D>& GetDiffuseMap() const { return m_Material->DiffuseMap; }
		const Reference<Texture2D>& GetSpecularMap() const { return m_Material->SpecularMap; }
		const Reference<Texture2D>& GetNormalMap() const { return m_Material->NormalMap; }

	private:
		void setupMesh();

	private:
		std::vector<Vertex_t> m_Vertices;
		std::vector<unsigned int> m_Indices;
		Reference<Material> m_Material;

		Reference<VertexArray> m_VertexArray;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;

	private:
		PrimitiveType m_Type = PrimitiveType::None;
		PrimitiveParams m_Params;

		friend class MeshBuilder;
	};

}