#include "MonsiPch.h"
#include "MeshBuilder.h"

namespace Monsi {

	Reference<Mesh> MeshBuilder::CreateSphere(float radius, uint32_t rings, uint32_t sectors, const Reference<Material>& material)
	{
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

		return CreateReference<Mesh>(vertices, indices, material);
	}

	Reference<Mesh> MeshBuilder::CreateGrid(float width, float depth, uint32_t columns, uint32_t rows, const Reference<Material>& material)
	{
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

		return CreateReference<Mesh>(vertices, indices, material);
	}
}