#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "ModelImportSettings.h"

namespace Monsi {

	struct BoneInfo {
		int id;
		glm::mat4 offsetMatrix;
	};

	class AnimatedModel
	{
	public:
		AnimatedModel(const std::string& filepath);
		AnimatedModel(const std::string& filepath, const ModelImportSettings& settings);

		void LoadModel(const std::string& filepath);
		void LoadModel(const std::string& filepath, const ModelImportSettings& settings);

		void UpdateAnimation(float deltaTime);

		std::string GetFilePath() const { return m_FilePath; }
		const std::vector<AnimatedMesh>& GetMeshes() const { return m_Meshes; }
		const std::vector<glm::mat4>& GetFinalBoneTransforms() const { return m_FinalBoneTransforms; }

		void SetModelSettings(const ModelImportSettings& settings) { m_ModelSettings = settings; }
		const ModelImportSettings& GetModelSettings() const { return m_ModelSettings; }

	private:
		void processNode(aiNode* node, const aiScene* scene);
		AnimatedMesh processMesh(aiMesh* mesh, const aiScene* scene);
		void extractBoneWeights(std::vector<AnimatedVertex>& vertices, aiMesh* mesh);
		void updateNodeHierarchy(float animationTime, const aiNode* pNode, const glm::mat4& parentTransform);
		const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string& nodeName);
		Reference<Texture2D> loadMaterialTexture(aiMaterial* mat, aiTextureType type);

	private:
		ModelImportSettings m_ModelSettings;
		std::vector<AnimatedMesh> m_Meshes;
		std::unordered_map<std::string, Reference<Texture2D>> m_TextureCache;
		std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
		std::vector<glm::mat4> m_FinalBoneTransforms;

		Assimp::Importer m_Importer;
		const aiScene* m_Scene = nullptr;
		glm::mat4 m_GlobalInverseTransform = glm::mat4(1.0f);
		float m_CurrentAnimTime = 0.0f;
		std::string m_Directory;
		std::string m_FilePath;
	};

}