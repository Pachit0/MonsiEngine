#pragma once

#include "ModelLoader.h"
#include "Lighting.h"
#include "ShadowMap.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>
#include <unordered_set>
#include <unordered_map>

namespace Monsi {

	class ModelPass {
	public:
		void Init();
		void Shutdown();

		void BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting);
		void EndScene();

		void SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap);

		void DrawModel(const Reference<Model>& model, const glm::mat4& transform, const glm::vec4& color);
		void DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		void DrawModel(const Reference<Model>& model, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);

		void DrawMesh(const Mesh* meshPtr, const glm::mat4& transform, const glm::vec4& color);
		void DrawMesh(const Mesh* meshPtr, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);

		void ClearBatches();

	private:
		void Flush();
		void RegisterMesh(const Mesh* mesh);

		void PruneStaleBatches();

	private:
		struct ModelInstanceData
		{
			glm::mat4 Transform;
			glm::vec4 Color;
		};

		struct MeshBatch
		{
			const Mesh* MeshPtr = nullptr;
			std::weak_ptr<void> LifetimeToken;
			std::vector<ModelInstanceData> InstanceData;
			bool WarnedOverflow = false;
		};

		static constexpr uint32_t MaxInstances = 10000;
		static constexpr uint32_t DefaultBatchReserve = 64;
		static constexpr uint32_t ShadowMapTextureSlot = 3;

		std::unordered_map<uint64_t, MeshBatch> m_MeshBatches;
		std::vector<MeshBatch*> m_FlushList;

		Reference<VertexBuffer> m_InstanceVBO;
		Reference<Shader> m_Shader;
		Reference<Texture2D> m_WhiteTexture;

		glm::mat4 m_ViewProjection;
	};

}