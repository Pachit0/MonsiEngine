#include "MonsiPch.h"
#include "AnimatedModel.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Monsi {

	static glm::mat4 ConvertAssimpMatrix(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	static uint32_t FindPositionKey(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (uint32_t i = 0; i < nodeAnim->mNumPositionKeys - 1; ++i)
		{
			if (animationTime < static_cast<float>(nodeAnim->mPositionKeys[i + 1].mTime))
				return i;
		}
		return nodeAnim->mNumPositionKeys - 2;
	}

	static uint32_t FindRotationKey(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (uint32_t i = 0; i < nodeAnim->mNumRotationKeys - 1; ++i)
		{
			if (animationTime < static_cast<float>(nodeAnim->mRotationKeys[i + 1].mTime))
				return i;
		}
		return nodeAnim->mNumRotationKeys - 2;
	}

	static uint32_t FindScalingKey(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (uint32_t i = 0; i < nodeAnim->mNumScalingKeys - 1; ++i)
		{
			if (animationTime < static_cast<float>(nodeAnim->mScalingKeys[i + 1].mTime))
				return i;
		}
		return nodeAnim->mNumScalingKeys - 2;
	}

	static aiVector3D CalcInterpolatedPosition(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumPositionKeys == 1)
			return nodeAnim->mPositionKeys[0].mValue;

		uint32_t index = FindPositionKey(animationTime, nodeAnim);
		uint32_t nextIndex = index + 1;
		float deltaTime = static_cast<float>(nodeAnim->mPositionKeys[nextIndex].mTime - nodeAnim->mPositionKeys[index].mTime);
		float factor = (animationTime - static_cast<float>(nodeAnim->mPositionKeys[index].mTime)) / deltaTime;
		factor = glm::clamp(factor, 0.0f, 1.0f);

		const aiVector3D& start = nodeAnim->mPositionKeys[index].mValue;
		const aiVector3D& end = nodeAnim->mPositionKeys[nextIndex].mValue;
		return start + factor * (end - start);
	}

	static aiQuaternion CalcInterpolatedRotation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumRotationKeys == 1)
			return nodeAnim->mRotationKeys[0].mValue;

		uint32_t index = FindRotationKey(animationTime, nodeAnim);
		uint32_t nextIndex = index + 1;
		float deltaTime = static_cast<float>(nodeAnim->mRotationKeys[nextIndex].mTime - nodeAnim->mRotationKeys[index].mTime);
		float factor = (animationTime - static_cast<float>(nodeAnim->mRotationKeys[index].mTime)) / deltaTime;
		factor = glm::clamp(factor, 0.0f, 1.0f);

		const aiQuaternion& start = nodeAnim->mRotationKeys[index].mValue;
		const aiQuaternion& end = nodeAnim->mRotationKeys[nextIndex].mValue;
		aiQuaternion out;
		aiQuaternion::Interpolate(out, start, end, factor);
		return out.Normalize();
	}

	static aiVector3D CalcInterpolatedScaling(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumScalingKeys == 1)
			return nodeAnim->mScalingKeys[0].mValue;

		uint32_t index = FindScalingKey(animationTime, nodeAnim);
		uint32_t nextIndex = index + 1;
		float deltaTime = static_cast<float>(nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
		float factor = (animationTime - static_cast<float>(nodeAnim->mScalingKeys[index].mTime)) / deltaTime;
		factor = glm::clamp(factor, 0.0f, 1.0f);

		const aiVector3D& start = nodeAnim->mScalingKeys[index].mValue;
		const aiVector3D& end = nodeAnim->mScalingKeys[nextIndex].mValue;
		return start + factor * (end - start);
	}

	AnimatedModel::AnimatedModel(const std::string& filepath)
	{
		LoadModel(filepath);
	}

	AnimatedModel::AnimatedModel(const std::string& filepath, const ModelImportSettings& settings)
	{
		LoadModel(filepath, settings);
	}

	void AnimatedModel::LoadModel(const std::string& filepath)
	{
		LoadModel(filepath, ModelImportSettings{});
	}

	void AnimatedModel::LoadModel(const std::string& filepath, const ModelImportSettings& settings)
	{
		m_ModelSettings = settings;

		m_Meshes.clear();
		m_TextureCache.clear();
		m_BoneInfoMap.clear();
		m_FinalBoneTransforms.clear();

		m_CurrentAnimIndex = 0;
		m_CurrentAnimTime = 0.0f;
		m_AnimationSpeed = 1.0f;
		m_IsPaused = false;


		unsigned int flags = aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_PopulateArmatureData;

		if (settings.GenSmoothNormals) flags |= aiProcess_GenSmoothNormals;
		if (settings.CalcTangentSpace) flags |= aiProcess_CalcTangentSpace;
		if (settings.FlipUVs)          flags |= aiProcess_FlipUVs;

		m_Scene = m_Importer.ReadFile(filepath, flags);

		if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode)
		{
			ENGINE_LOG_ERROR("ASSIMP ERROR: {0}", m_Importer.GetErrorString());
			ENGINE_ASSERT(false, "AnimatedModel load failed!");
			return;
		}

		m_FilePath = filepath;
		m_Directory = filepath.substr(0, filepath.find_last_of("/\\"));
		m_GlobalInverseTransform = glm::inverse(ConvertAssimpMatrix(m_Scene->mRootNode->mTransformation));

		m_Meshes.reserve(m_Scene->mNumMeshes);
		processNode(m_Scene->mRootNode, m_Scene);

		m_FinalBoneTransforms.resize(m_BoneInfoMap.size(), glm::mat4(1.0f));
	}

	void AnimatedModel::processNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	AnimatedMesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<AnimatedVertex> vertices(mesh->mNumVertices);

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			vertices[i].Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertices[i].Normal = { mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z };

			if (mesh->mTextureCoords[0])
				vertices[i].TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			else
				vertices[i].TexCoords = { 0.0f, 0.0f };
		}

		extractBoneWeights(vertices, mesh);

		size_t totalIndices = 0;
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			totalIndices += mesh->mFaces[i].mNumIndices;

		std::vector<unsigned int> indices;
		indices.reserve(totalIndices);

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		auto meshMaterial = CreateReference<Material>();

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);
			if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
				meshMaterial->DiffuseColor = glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0f);

			aiColor3D specularColor(1.0f, 1.0f, 1.0f);
			if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
				meshMaterial->SpecularColor = glm::vec3(specularColor.r, specularColor.g, specularColor.b);

			float shininess = 32.0f;
			if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
				meshMaterial->Shininess = shininess;

			int twoSided = 0;
			if (material->Get(AI_MATKEY_TWOSIDED, twoSided) == AI_SUCCESS)
				meshMaterial->DoubleSided = (twoSided != 0);

			meshMaterial->DiffuseMap = loadMaterialTexture(material, aiTextureType_DIFFUSE);
			meshMaterial->SpecularMap = loadMaterialTexture(material, aiTextureType_SPECULAR);
			meshMaterial->NormalMap = loadMaterialTexture(material, aiTextureType_HEIGHT);
		}

		return AnimatedMesh(vertices, indices, meshMaterial);
	}

	void AnimatedModel::extractBoneWeights(std::vector<AnimatedVertex>& vertices, aiMesh* mesh)
	{
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

			if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
			{
				BoneInfo newBoneInfo;
				newBoneInfo.id = static_cast<int>(m_BoneInfoMap.size());
				newBoneInfo.offsetMatrix = ConvertAssimpMatrix(mesh->mBones[boneIndex]->mOffsetMatrix);
				m_BoneInfoMap[boneName] = newBoneInfo;
				boneID = newBoneInfo.id;
			}
			else
			{
				boneID = m_BoneInfoMap[boneName].id;
			}

			auto weights = mesh->mBones[boneIndex]->mWeights;
			uint32_t numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (uint32_t weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				uint32_t vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				vertices[vertexId].AddBoneData(boneID, weight);
			}
		}
	}

	void AnimatedModel::UpdateAnimation(float deltaTime)
	{
		if (!m_Scene || !m_Scene->HasAnimations() || m_CurrentAnimIndex >= m_Scene->mNumAnimations)
			return;

		if (m_IsPaused)
			return;

		aiAnimation* anim = m_Scene->mAnimations[m_CurrentAnimIndex];

		if (anim->mDuration <= 0.0)
			return;

		float ticksPerSecond = static_cast<float>(anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 25.0f);

		m_CurrentAnimTime += deltaTime * ticksPerSecond * m_AnimationSpeed;
		m_CurrentAnimTime = fmod(m_CurrentAnimTime, static_cast<float>(anim->mDuration));
		if (m_CurrentAnimTime < 0.0f)
			m_CurrentAnimTime += static_cast<float>(anim->mDuration);

		updateNodeHierarchy(m_CurrentAnimTime, m_Scene->mRootNode, glm::mat4(1.0f));
	}

	std::string AnimatedModel::GetAnimationName(uint32_t index) const
	{
		if (!m_Scene || index >= m_Scene->mNumAnimations) return "";
		return m_Scene->mAnimations[index]->mName.C_Str();
	}

	void AnimatedModel::SetAnimationSpeed(float speed)
	{
		m_AnimationSpeed = glm::clamp(speed, 0.0f, 10.0f);
	}

	void AnimatedModel::SetAnimation(uint32_t index)
	{
		if (!m_Scene || index >= m_Scene->mNumAnimations)
		{
			ENGINE_LOG_ERROR("Invalid animation index: {0}", index);
			return;
		}

		if (index != m_CurrentAnimIndex)
		{
			m_CurrentAnimIndex = index;
			m_CurrentAnimTime = 0.0f;
		}
	}

	void AnimatedModel::updateNodeHierarchy(float animationTime, const aiNode* pNode, const glm::mat4& parentTransform)
	{
		std::string nodeName(pNode->mName.data);
		glm::mat4 nodeTransform = ConvertAssimpMatrix(pNode->mTransformation);

		if (m_Scene && m_Scene->HasAnimations() && m_CurrentAnimIndex < m_Scene->mNumAnimations)
		{
			const aiNodeAnim* nodeAnim = findNodeAnim(m_Scene->mAnimations[m_CurrentAnimIndex], nodeName);

			if (nodeAnim)
			{
				aiVector3D scale, position;
				aiQuaternion rotation;
				pNode->mTransformation.Decompose(scale, rotation, position);

				if (nodeAnim->mNumScalingKeys > 0) {
					scale = CalcInterpolatedScaling(animationTime, nodeAnim);
				}

				if (nodeAnim->mNumRotationKeys > 0) {
					rotation = CalcInterpolatedRotation(animationTime, nodeAnim);
				}

				if (nodeAnim->mNumPositionKeys > 0) {
					position = CalcInterpolatedPosition(animationTime, nodeAnim);
				}

				glm::mat4 T = glm::translate(glm::mat4(1.0f),glm::vec3(position.x, position.y, position.z));
				glm::mat4 R = glm::toMat4(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
				glm::mat4 S = glm::scale(glm::mat4(1.0f),glm::vec3(scale.x, scale.y, scale.z));

				nodeTransform = T * R * S;
			}
		}

		glm::mat4 globalTransform = parentTransform * nodeTransform;

		if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
		{
			int boneIndex = m_BoneInfoMap[nodeName].id;
			glm::mat4 offset = m_BoneInfoMap[nodeName].offsetMatrix;
			m_FinalBoneTransforms[boneIndex] = m_GlobalInverseTransform * globalTransform * offset;
		}

		for (uint32_t i = 0; i < pNode->mNumChildren; ++i)
		{
			updateNodeHierarchy(animationTime, pNode->mChildren[i], globalTransform);
		}
	}

	const aiNodeAnim* AnimatedModel::findNodeAnim(const aiAnimation* animation, const std::string& nodeName)
	{
		for (uint32_t i = 0; i < animation->mNumChannels; ++i)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName) return nodeAnim;
		}
		return nullptr;
	}

	Reference<Texture2D> AnimatedModel::loadMaterialTexture(aiMaterial* mat, aiTextureType type)
	{
		if (mat->GetTextureCount(type) == 0) return nullptr;

		aiString str;
		mat->GetTexture(type, 0, &str);
		std::string path = str.C_Str();

		auto it = m_TextureCache.find(path);
		if (it != m_TextureCache.end())
		{
			return it->second;
		}

		std::string filename = m_Directory + "/" + path;
		Reference<Texture2D> texture = Texture2D::Create(filename);

		m_TextureCache[path] = texture;
		return texture;
	}

}