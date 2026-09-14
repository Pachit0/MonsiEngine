#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"
#include "ModelImportSettings.h"

namespace Monsi {

	class StaticModel
	{
	public:
		StaticModel(const std::string& filepath);
		StaticModel(const std::string& filepath, const ModelImportSettings& settings);

		void LoadModel(const std::string& filepath);
		void LoadModel(const std::string& filepath, const ModelImportSettings& settings);

		std::string GetFilePath() const { return m_FilePath; }
		const std::vector<StaticMesh>& GetMeshes() const { return m_Meshes; }

		void SetModelSettings(const ModelImportSettings& settings) { m_ModelSettings = settings; }
		const ModelImportSettings& GetModelSettings() const { return m_ModelSettings; }

	private:
		void processNode(aiNode* node, const aiScene* scene);
		StaticMesh processMesh(aiMesh* mesh, const aiScene* scene);
		Reference<Texture2D> loadMaterialTexture(aiMaterial* mat, aiTextureType type);

	private:
		ModelImportSettings m_ModelSettings;
		std::vector<StaticMesh> m_Meshes;
		std::unordered_map<std::string, Reference<Texture2D>> m_TextureCache;
		std::string m_Directory;
		std::string m_FilePath;
	};

}