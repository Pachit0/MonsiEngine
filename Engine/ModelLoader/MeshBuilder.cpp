#include "MonsiPch.h"
#include "MeshBuilder.h"

namespace Monsi {

	static	void AddQuadFace(std::vector<Vertex_t>& vertices, std::vector<uint32_t>& indices,
		const glm::vec3& center, const glm::vec3& normal,
		const glm::vec3& right, const glm::vec3& up,
		float halfWidth, float halfHeight)
	{
		uint32_t base = (uint32_t)vertices.size();

		Vertex_t vertex;
		vertex.Normal = normal;

		vertex.Position = center - right * halfWidth + up * halfHeight;
		vertex.TexCoords = { 0.0f, 0.0f };
		vertices.push_back(vertex);

		vertex.Position = center + right * halfWidth + up * halfHeight;
		vertex.TexCoords = { 1.0f, 0.0f };
		vertices.push_back(vertex);

		vertex.Position = center - right * halfWidth - up * halfHeight;
		vertex.TexCoords = { 0.0f, 1.0f };
		vertices.push_back(vertex);

		vertex.Position = center + right * halfWidth - up * halfHeight;
		vertex.TexCoords = { 1.0f, 1.0f };
		vertices.push_back(vertex);

		indices.push_back(base + 0);
		indices.push_back(base + 2);
		indices.push_back(base + 1);

		indices.push_back(base + 1);
		indices.push_back(base + 2);
		indices.push_back(base + 3);
	}

	static void AddDiskCap(std::vector<Vertex_t>& vertices, std::vector<uint32_t>& indices,
		float y, float radius, uint32_t sectors, const glm::vec3& normal, float windingSign)
	{
		const float pi = 3.1415926535f;

		uint32_t centerIndex = (uint32_t)vertices.size();

		Vertex_t centerVertex;
		centerVertex.Position = glm::vec3(0.0f, y, 0.0f);
		centerVertex.Normal = normal;
		centerVertex.TexCoords = { 0.5f, 0.5f };
		vertices.push_back(centerVertex);

		uint32_t rimStart = (uint32_t)vertices.size();

		for (uint32_t s = 0; s <= sectors; ++s)
		{
			float phi = (float)s * 2.0f * pi / (float)sectors;
			float x = std::cos(phi);
			float z = std::sin(phi);

			Vertex_t vertex;
			vertex.Position = glm::vec3(x * radius, y, z * radius);
			vertex.Normal = normal;
			vertex.TexCoords = { x * 0.5f + 0.5f, z * 0.5f + 0.5f };
			vertices.push_back(vertex);
		}

		for (uint32_t s = 0; s < sectors; ++s)
		{
			uint32_t a = rimStart + s;
			uint32_t b = rimStart + s + 1;

			if (windingSign >= 0.0f)
			{
				indices.push_back(centerIndex);
				indices.push_back(b);
				indices.push_back(a);
			}
			else
			{
				indices.push_back(centerIndex);
				indices.push_back(a);
				indices.push_back(b);
			}
		}
	}

	Reference<Mesh> MeshBuilder::CreateFromParams(const PrimitiveParams& params, const Reference<Material>& material)
	{
		return std::visit([&material](auto&& p) -> Reference<Mesh>
			{
				using T = std::decay_t<decltype(p)>;
				if constexpr (std::is_same_v<T, std::monostate>)
				{
					return nullptr;
				}
				else
				{
					return MeshBuilder::Create(p, material);
				}
			}, params);
	}

	Reference<Mesh> MeshBuilder::CreateSphere(float radius, uint32_t rings, uint32_t sectors, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve((rings + 1) * (sectors + 1));
		indices.reserve(rings * sectors * 6);

		const float pi = 3.1415926535f;

		for (uint32_t r = 0; r <= rings; ++r)
		{
			float theta = (float)r * pi / (float)rings;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			for (uint32_t s = 0; s <= sectors; ++s)
			{
				float phi = (float)s * 2.0f * pi / (float)sectors;
				float sinPhi = std::sin(phi);
				float cosPhi = std::cos(phi);

				Vertex_t vertex;

				vertex.Normal.x = cosPhi * sinTheta;
				vertex.Normal.y = cosTheta;
				vertex.Normal.z = sinPhi * sinTheta;

				vertex.Position = vertex.Normal * radius;

				vertex.TexCoords.x = (float)s / (float)sectors;
				vertex.TexCoords.y = (float)r / (float)rings;

				vertices.push_back(vertex);
			}
		}

		for (uint32_t r = 0; r < rings; ++r)
		{
			for (uint32_t s = 0; s < sectors; ++s)
			{
				uint32_t topLeft = r * (sectors + 1) + s;
				uint32_t topRight = topLeft + 1;
				uint32_t bottomLeft = (r + 1) * (sectors + 1) + s;
				uint32_t bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);

				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(bottomRight);
			}
		}

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Sphere;

		return meshBuild;
	}

	Reference<Mesh> MeshBuilder::CreateGrid(float width, float depth, uint32_t columns, uint32_t rows, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve((rows + 1) * (columns + 1));
		indices.reserve(rows * columns * 6);

		float halfWidth = width / 2.0f;
		float halfDepth = depth / 2.0f;

		float dx = width / (float)columns;
		float dz = depth / (float)rows;

		for (uint32_t r = 0; r <= rows; ++r)
		{
			float z = halfDepth - (float)r * dz;

			for (uint32_t c = 0; c <= columns; ++c)
			{
				float x = -halfWidth + (float)c * dx;

				Vertex_t vertex;
				vertex.Position = glm::vec3(x, 0.0f, z);
				vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
				vertex.TexCoords.x = (float)c / (float)columns;
				vertex.TexCoords.y = (float)r / (float)rows;

				vertices.push_back(vertex);
			}
		}

		uint32_t verticesPerRow = columns + 1;

		for (uint32_t r = 0; r < rows; ++r)
		{
			for (uint32_t c = 0; c < columns; ++c)
			{
				uint32_t topLeft = r * verticesPerRow + c;
				uint32_t topRight = topLeft + 1;
				uint32_t bottomLeft = (r + 1) * verticesPerRow + c;
				uint32_t bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Grid;

		return meshBuild;
	}

	Reference<Mesh> MeshBuilder::CreateCube(float size, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve(24);
		indices.reserve(36);

		float half = size * 0.5f;

		AddQuadFace(vertices, indices, glm::vec3(half, 0.0f, 0.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), half, half);
		AddQuadFace(vertices, indices, glm::vec3(-half, 0.0f, 0.0f), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), half, half);
		AddQuadFace(vertices, indices, glm::vec3(0.0f, half, 0.0f), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), half, half);
		AddQuadFace(vertices, indices, glm::vec3(0.0f, -half, 0.0f), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), half, half);
		AddQuadFace(vertices, indices, glm::vec3(0.0f, 0.0f, half), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), half, half);
		AddQuadFace(vertices, indices, glm::vec3(0.0f, 0.0f, -half), glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), half, half);

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Cube;

		return meshBuild;
	}

	Reference<Mesh> MeshBuilder::CreateCylinder(float radius, float height, uint32_t sectors, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve((sectors + 1) * 2 + (sectors + 1) * 2 + 2);
		indices.reserve(sectors * 6 * 3);

		const float pi = 3.1415926535f;
		float halfHeight = height * 0.5f;

		for (uint32_t s = 0; s <= sectors; ++s)
		{
			float phi = (float)s * 2.0f * pi / (float)sectors;
			float x = std::cos(phi);
			float z = std::sin(phi);
			float u = (float)s / (float)sectors;

			Vertex_t top;
			top.Position = glm::vec3(x * radius, halfHeight, z * radius);
			top.Normal = glm::vec3(x, 0.0f, z);
			top.TexCoords = { u, 0.0f };
			vertices.push_back(top);

			Vertex_t bottom;
			bottom.Position = glm::vec3(x * radius, -halfHeight, z * radius);
			bottom.Normal = glm::vec3(x, 0.0f, z);
			bottom.TexCoords = { u, 1.0f };
			vertices.push_back(bottom);
		}

		for (uint32_t s = 0; s < sectors; ++s)
		{
			uint32_t topLeft = s * 2;
			uint32_t bottomLeft = s * 2 + 1;
			uint32_t topRight = (s + 1) * 2;
			uint32_t bottomRight = (s + 1) * 2 + 1;

			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);

			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(bottomRight);
		}

		AddDiskCap(vertices, indices, halfHeight, radius, sectors, glm::vec3(0, 1, 0), 1.0f);
		AddDiskCap(vertices, indices, -halfHeight, radius, sectors, glm::vec3(0, -1, 0), -1.0f);

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Cylinder;

		return meshBuild;
	}

	Reference<Mesh> MeshBuilder::CreateCone(float radius, float height, uint32_t sectors, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve((sectors + 1) * 2 + (sectors + 1) + 1);
		indices.reserve(sectors * 3 + sectors * 3);

		const float pi = 3.1415926535f;
		float halfHeight = height * 0.5f;

		for (uint32_t s = 0; s <= sectors; ++s)
		{
			float phi = (float)s * 2.0f * pi / (float)sectors;
			float cosPhi = std::cos(phi);
			float sinPhi = std::sin(phi);
			float u = (float)s / (float)sectors;

			glm::vec3 normal = glm::normalize(glm::vec3(height * cosPhi, radius, height * sinPhi));

			Vertex_t apex;
			apex.Position = glm::vec3(0.0f, halfHeight, 0.0f);
			apex.Normal = normal;
			apex.TexCoords = { u, 0.0f };
			vertices.push_back(apex);

			Vertex_t base;
			base.Position = glm::vec3(cosPhi * radius, -halfHeight, sinPhi * radius);
			base.Normal = normal;
			base.TexCoords = { u, 1.0f };
			vertices.push_back(base);
		}

		for (uint32_t s = 0; s < sectors; ++s)
		{
			uint32_t apexLeft = s * 2;
			uint32_t baseLeft = s * 2 + 1;
			uint32_t baseRight = (s + 1) * 2 + 1;

			indices.push_back(apexLeft);
			indices.push_back(baseRight);
			indices.push_back(baseLeft);
		}

		AddDiskCap(vertices, indices, -halfHeight, radius, sectors, glm::vec3(0, -1, 0), -1.0f);

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Cone;

		return meshBuild;
	}

	Reference<Mesh> MeshBuilder::CreateTorus(float majorRadius, float minorRadius, uint32_t majorSegments, uint32_t minorSegments, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve((majorSegments + 1) * (minorSegments + 1));
		indices.reserve(majorSegments * minorSegments * 6);

		const float pi = 3.1415926535f;

		for (uint32_t i = 0; i <= majorSegments; ++i)
		{
			float theta = (float)i * 2.0f * pi / (float)majorSegments;
			float cosTheta = std::cos(theta);
			float sinTheta = std::sin(theta);

			for (uint32_t j = 0; j <= minorSegments; ++j)
			{
				float phi = (float)j * 2.0f * pi / (float)minorSegments;
				float cosPhi = std::cos(phi);
				float sinPhi = std::sin(phi);

				float tubeCenterOffset = majorRadius + minorRadius * cosPhi;

				Vertex_t vertex;
				vertex.Position.x = tubeCenterOffset * cosTheta;
				vertex.Position.y = minorRadius * sinPhi;
				vertex.Position.z = tubeCenterOffset * sinTheta;

				vertex.Normal = glm::vec3(cosPhi * cosTheta, sinPhi, cosPhi * sinTheta);

				vertex.TexCoords.x = (float)i / (float)majorSegments;
				vertex.TexCoords.y = (float)j / (float)minorSegments;

				vertices.push_back(vertex);
			}
		}

		uint32_t verticesPerRow = minorSegments + 1;

		for (uint32_t i = 0; i < majorSegments; ++i)
		{
			for (uint32_t j = 0; j < minorSegments; ++j)
			{
				uint32_t topLeft = i * verticesPerRow + j;
				uint32_t topRight = topLeft + 1;
				uint32_t bottomLeft = (i + 1) * verticesPerRow + j;
				uint32_t bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);

				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(bottomRight);
			}
		}

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Torus;

		return meshBuild;
	}

	Reference<Mesh> MeshBuilder::CreateQuad(float width, float height, const Reference<Material>& material)
	{
		Reference<Mesh> meshBuild;
		std::vector<Vertex_t> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve(4);
		indices.reserve(6);

		AddQuadFace(vertices, indices, glm::vec3(0.0f), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), width * 0.5f, height * 0.5f);

		meshBuild = CreateReference<Mesh>(vertices, indices, material);
		meshBuild->m_Type = PrimitiveType::Quad;

		return meshBuild;
	}

	Monsi::PrimitiveParams MeshBuilder::MakeDefaultParams(PrimitiveType type)
	{
		switch (type)
		{
		case PrimitiveType::Sphere:   return SphereParams{};
		case PrimitiveType::Grid:     return GridParams{};
		case PrimitiveType::Cube:     return CubeParams{};
		case PrimitiveType::Cylinder: return CylinderParams{};
		case PrimitiveType::Cone:     return ConeParams{};
		case PrimitiveType::Torus:    return TorusParams{};
		case PrimitiveType::Quad:     return QuadParams{};
		case PrimitiveType::None:
		default:                      return std::monostate{};
		}
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const SphereParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateSphere(p.radius, p.rings, p.sectors, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const CubeParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateCube(p.size, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const GridParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateGrid(p.width, p.depth, p.columns, p.rows, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const CylinderParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateCylinder(p.radius, p.height, p.sectors, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const ConeParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateCone(p.radius, p.height, p.sectors, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const TorusParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateTorus(p.majorRadius, p.minorRadius, p.majorSegments, p.minorSegments, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

	Monsi::Reference<Monsi::Mesh> MeshBuilder::Create(const QuadParams& p, const Reference<Material>& mat)
	{
		auto mesh = CreateQuad(p.width, p.height, mat);
		mesh->SetPrimitiveParams(p);
		return mesh;
	}

}