#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

namespace Monsi {

	struct ModelImportSettings {
		bool FlipUVs = true;
		bool GenSmoothNormals = true;
		bool CalcTangentSpace = true;
	};

	class Model
	{
	public:
		Model(const std::string& filepath);
		Model(const std::string& filepath, const ModelImportSettings& settings);

		void LoadModel(const std::string& filepath);
		void LoadModel(const std::string& filepath, const ModelImportSettings& settings);

		std::string GetFilePath() const { return m_FilePath; }

		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
		
		void SetModelSettings(const ModelImportSettings& settings) { m_ModelSettings = settings; }
		const ModelImportSettings& GetModelSettings() const { return m_ModelSettings; }
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		Reference<Texture2D> LoadMaterialTexture(aiMaterial* mat, aiTextureType type);

	private:
		ModelImportSettings m_ModelSettings;
		std::vector<Mesh> m_Meshes;
		std::unordered_map<std::string, Reference<Texture2D>> m_TextureCache;
		std::string m_Directory;
		std::string m_FilePath;
	};

}