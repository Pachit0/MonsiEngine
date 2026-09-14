#pragma once

#include "Mesh.h"
#include "PrimitiveParams.h"
#include <cmath>

namespace Monsi {

	class MeshBuilder {
	public:
		static Reference<StaticMesh> Create(const SphereParams& p, const Reference<Material>& mat);
		static Reference<StaticMesh> Create(const CubeParams& p, const Reference<Material>& mat);
		static Reference<StaticMesh> Create(const GridParams& p, const Reference<Material>& mat);
		static Reference<StaticMesh> Create(const CylinderParams& p, const Reference<Material>& mat);
		static Reference<StaticMesh> Create(const ConeParams& p, const Reference<Material>& mat);
		static Reference<StaticMesh> Create(const TorusParams& p, const Reference<Material>& mat);
		static Reference<StaticMesh> Create(const QuadParams& p, const Reference<Material>& mat);

		static Reference<StaticMesh> CreateFromParams(const PrimitiveParams& params, const Reference<Material>& material);
		static PrimitiveParams MakeDefaultParams(PrimitiveType type);
	
	private:
		static Reference<StaticMesh> CreateSphere(float radius, uint32_t rings, uint32_t sectors, const Reference<Material>& material);
		static Reference<StaticMesh> CreateGrid(float width, float depth, uint32_t columns, uint32_t rows, const Reference<Material>& material);
		static Reference<StaticMesh> CreateCube(float size, const Reference<Material>& material);
		static Reference<StaticMesh> CreateCylinder(float radius, float height, uint32_t sectors, const Reference<Material>& material);
		static Reference<StaticMesh> CreateCone(float radius, float height, uint32_t sectors, const Reference<Material>& material);
		static Reference<StaticMesh> CreateTorus(float majorRadius, float minorRadius, uint32_t majorSegments, uint32_t minorSegments, const Reference<Material>& material);
		static Reference<StaticMesh> CreateQuad(float width, float height, const Reference<Material>& material);
	};

}

