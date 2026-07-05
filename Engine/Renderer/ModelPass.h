#pragma once

#include "ModelLoader.h"
#include "Lighting.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <unordered_set>
#include <unordered_map>

namespace Monsi {

	class ModelPass {
	public:
		void Init();
		void Shutdown();

		void BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting);
		void EndScene();

		void DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color);
		void DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		void DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);

		void DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color);
		void DrawMesh(const Mesh* meshPtr, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);

	private:
		void Flush();
		void RegisterMesh(const Mesh* mesh);

	private:
		struct ModelInstanceData
		{
			glm::mat4 Transform;
			glm::vec4 Color;
		};

		struct MeshBatch
		{
			const Mesh* MeshPtr = nullptr;
			std::vector<ModelInstanceData> InstanceData;

			ModelInstanceData* Instances = nullptr;
			ModelInstanceData* Cursor = nullptr;
		};

		static constexpr uint32_t MaxInstances = 10000;

		std::unordered_map<const Mesh*, MeshBatch> m_MeshBatches;
		std::unordered_set<const Mesh*> m_RegisteredMeshes;

		ModelInstanceData* m_InstanceBuffer = nullptr;
		ModelInstanceData* m_BufferCursor = nullptr;

		Reference<VertexBuffer> m_InstanceVBO;
		Reference<Shader> m_Shader;
		Reference<Texture2D> m_WhiteTexture;

		glm::mat4 m_ViewProjection;
	};

}