#pragma once

#include "Mesh.h"
#include "PrimitiveParams.h"
#include <cmath>

namespace Monsi {

	class MeshBuilder {
	public:
		static Reference<Mesh> MeshBuilder::Create(const SphereParams& p, const Reference<Material>& mat);
		static Reference<Mesh> MeshBuilder::Create(const CubeParams& p, const Reference<Material>& mat);
		static Reference<Mesh> MeshBuilder::Create(const GridParams& p, const Reference<Material>& mat);
		static Reference<Mesh> MeshBuilder::Create(const CylinderParams& p, const Reference<Material>& mat);
		static Reference<Mesh> MeshBuilder::Create(const ConeParams& p, const Reference<Material>& mat);
		static Reference<Mesh> MeshBuilder::Create(const TorusParams& p, const Reference<Material>& mat);
		static Reference<Mesh> MeshBuilder::Create(const QuadParams& p, const Reference<Material>& mat);

		static Reference<Mesh> CreateFromParams(const PrimitiveParams& params, const Reference<Material>& material);
		static PrimitiveParams MakeDefaultParams(PrimitiveType type);
	
	private:
		static Reference<Mesh> CreateSphere(float radius, uint32_t rings, uint32_t sectors, const Reference<Material>& material);
		static Reference<Mesh> CreateGrid(float width, float depth, uint32_t columns, uint32_t rows, const Reference<Material>& material);
		static Reference<Mesh> CreateCube(float size, const Reference<Material>& material);
		static Reference<Mesh> CreateCylinder(float radius, float height, uint32_t sectors, const Reference<Material>& material);
		static Reference<Mesh> CreateCone(float radius, float height, uint32_t sectors, const Reference<Material>& material);
		static Reference<Mesh> CreateTorus(float majorRadius, float minorRadius, uint32_t majorSegments, uint32_t minorSegments, const Reference<Material>& material);
		static Reference<Mesh> CreateQuad(float width, float height, const Reference<Material>& material);
	};

}

