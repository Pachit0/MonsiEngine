#pragma once

#include "Mesh.h"
#include <cmath>

namespace Monsi {

	class MeshBuilder {
	public:
		static Reference<Mesh> CreateSphere(float radius, uint32_t rings, uint32_t sectors, const Reference<Material>& material);
		static Reference<Mesh> CreateGrid(float width, float depth, uint32_t columns, uint32_t rows, const Reference<Material>& material);
	};

}

