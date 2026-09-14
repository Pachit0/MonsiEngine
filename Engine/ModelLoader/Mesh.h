#pragma once

#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "VertexArray.h"
#include "Material.h"
#include "PrimitiveParams.h"

namespace Monsi {

	enum class PrimitiveType {
		None = 0, Sphere, Grid, Cube, Cylinder, Cone, Torus, Quad
	};

	#define MAX_BONE_INFLUENCE 4

	struct StaticVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct AnimatedVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

		int BoneIDs[MAX_BONE_INFLUENCE] = { -1, -1, -1, -1 };
		float Weights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f, 0.0f };

		void AddBoneData(int boneID, float weight)
		{
			for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
			{
				if (BoneIDs[i] < 0)
				{
					BoneIDs[i] = boneID;
					Weights[i] = weight;
					return;
				}
			}
		}
	};

	class StaticMesh
	{
	public:
		StaticMesh() = default;
		StaticMesh(const std::vector<StaticVertex>& vertices, const std::vector<unsigned int>& indices, const Reference<Material>& material);

		const Reference<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_Indices.size()); }

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

		uint64_t GetId() const { return m_Id; }
		std::weak_ptr<void> GetLifetimeToken() const { return m_LifetimeToken; }

	private:
		void setupMesh();

	private:
		std::vector<StaticVertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		Reference<Material> m_Material;

		Reference<VertexArray> m_VertexArray;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;

		PrimitiveType m_Type = PrimitiveType::None;
		PrimitiveParams m_Params;

		static uint64_t s_NextId;
		uint64_t m_Id = s_NextId++;
		std::shared_ptr<char> m_LifetimeToken = std::make_shared<char>();

		friend class MeshBuilder;
	};

	class AnimatedMesh
	{
	public:
		AnimatedMesh() = default;
		AnimatedMesh(const std::vector<AnimatedVertex>& vertices, const std::vector<unsigned int>& indices, const Reference<Material>& material);

		const Reference<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_Indices.size()); }
		const Reference<Material>& GetMaterial() const { return m_Material; }

		uint64_t GetId() const { return m_Id; }
		std::weak_ptr<void> GetLifetimeToken() const { return m_LifetimeToken; }

	private:
		void setupMesh();

	private:
		std::vector<AnimatedVertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		Reference<Material> m_Material;

		Reference<VertexArray> m_VertexArray;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;

		static uint64_t s_NextId;
		uint64_t m_Id = s_NextId++;
		std::shared_ptr<char> m_LifetimeToken = std::make_shared<char>();
	};

}