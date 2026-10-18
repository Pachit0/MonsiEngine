#pragma once

namespace Monsi {

	struct ModelImportSettings {
		bool FlipUVs = true;
		bool GenSmoothNormals = true;
		bool CalcTangentSpace = true;
		bool PreTransformVertices = false;
		bool MakeLeftHanded = false;
		bool FlipWindingOrder = false;
		int CoordinateSystem = 0;
	};

}