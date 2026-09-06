#pragma once

#include <cstdint>
#include <variant>

namespace Monsi {

	struct SphereParams {
		float radius = 0.5f;
		uint32_t rings = 32;
		uint32_t sectors = 32;
	};

	struct GridParams {
		float width = 1.0f;
		float depth = 1.0f;
		uint32_t columns = 32;
		uint32_t rows = 32;
	};

	struct CubeParams {
		float size = 1.0f;
	};

	struct CylinderParams {
		float radius = 0.5f;
		float height = 1.0f;
		uint32_t sectors = 32;
	};

	struct ConeParams {
		float radius = 0.5f;
		float height = 1.0f;
		uint32_t sectors = 32;
	};

	struct TorusParams {
		float majorRadius = 0.5f;
		float minorRadius = 0.2f;
		uint32_t majorSegments = 32;
		uint32_t minorSegments = 16;
	};

	struct QuadParams {
		float width = 1.0f;
		float height = 1.0f;
	};

	using PrimitiveParams = std::variant<std::monostate, SphereParams, GridParams, CubeParams, CylinderParams, ConeParams, TorusParams, QuadParams>;


}