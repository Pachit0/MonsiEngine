#include "MonsiPch.h"
#include "ModelLoader.h"
#include "Material.h"

namespace Monsi {

	Model::Model(const std::string& filepath)
	{
		LoadModel(filepath);
	}

	Model::Model(const std::string& filepath, ModelImportSettings settings)
	{
		LoadModel(filepath, settings);
	}

	void Model::LoadModel(const std::string& filepath)
	{
		LoadModel(filepath, ModelImportSettings{});
	}

	void Model::LoadModel(const std::string& filepath, ModelImportSettings settings)
	{
		Assimp::Importer importer;
		unsigned int flags = aiProcess_Triangulate;

		if (settings.GenSmoothNormals) flags |= aiProcess_GenSmoothNormals;
		if (settings.CalcTangentSpace) flags |= aiProcess_CalcTangentSpace;
		if (settings.FlipUVs)          flags |= aiProcess_FlipUVs;

		const aiScene* scene = importer.ReadFile(filepath, flags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ENGINE_LOG_ERROR("ASSIMP ERROR: {0}", importer.GetErrorString());
			ENGINE_ASSERT(false, "Model load failed!");
			return;
		}

		m_Directory = filepath.substr(0, filepath.find_last_of("/\\"));
		m_Meshes.reserve(scene->mNumMeshes);

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
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

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex_t> vertices;
		vertices.reserve(mesh->mNumVertices);

		size_t totalIndices = 0;
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			totalIndices += mesh->mFaces[i].mNumIndices;

		std::vector<unsigned int> indices;
		indices.reserve(totalIndices);

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex_t vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z };

			if (mesh->mTextureCoords[0])
				vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			else
				vertex.TexCoords = { 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

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

			meshMaterial->DiffuseMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE);
			meshMaterial->SpecularMap = LoadMaterialTexture(material, aiTextureType_SPECULAR);
			meshMaterial->NormalMap = LoadMaterialTexture(material, aiTextureType_HEIGHT);
		}

		return Mesh(vertices, indices, meshMaterial);
	}

	Reference<Texture2D> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type)
	{
		if (mat->GetTextureCount(type) == 0)
			return nullptr;

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