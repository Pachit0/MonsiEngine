#include "MonsiPch.h"

#include "ModelLoader.h"

#include "Texture.h"

namespace Monsi {

	Model::Model(const std::string& filepath)
	{
		LoadModel(filepath);
	}

	void Model::LoadModel(const std::string& filepath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ENGINE_LOG_ERROR("ASSIMP ERROR: {0}", importer.GetErrorString());
			ENGINE_ASSERT(false, "Model load failed!");
			return;
		}

		m_Directory = filepath.substr(0, filepath.find_last_of("/\\"));

		m_TextureCache.reserve(scene->mNumMaterials * 4);
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

		std::vector<Texture_t> textures;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex_t vertex;

			vertex.Position =
			{
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};

			vertex.Normal =
			{
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};

			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoords =
				{
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else
			{
				vertex.TexCoords = { 0.0f,0.0f };
			}

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

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture_t> Model::loadMaterialTextures(
		aiMaterial* mat,
		aiTextureType type,
		const std::string& typeName)
	{
		const uint32_t textureCount = mat->GetTextureCount(type);

		std::vector<Texture_t> textures;
		textures.reserve(textureCount);

		for (uint32_t i = 0; i < textureCount; i++)
		{
			aiString str;

			mat->GetTexture(type, i, &str);

			const std::string path = str.C_Str();

			auto it = m_TextureCache.find(path);

			if (it != m_TextureCache.end())
			{
				textures.emplace_back(it->second);
			}
			else
			{
				Texture_t texture;

				std::string filename = m_Directory + "/" + path;

				texture.texture = Texture2D::Create(filename);
				texture.type = typeName;
				texture.path = path;

				textures.push_back(texture);
				m_TextureCache.emplace(path, texture);
			}
		}
		return textures;
	}

}