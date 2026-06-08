#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.h"

namespace Monsi {

	class Model
	{
	public:
		Model(const std::string& filepath);
		void LoadModel(const std::string& filepath);

		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture_t> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture_t> m_TexturesLoaded;
		std::string m_Directory;
	};

}