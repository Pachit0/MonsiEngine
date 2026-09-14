#pragma once

#include "AnimatedModel.h"
#include "Lighting.h"
#include "ShadowMap.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Monsi {

    class AnimatedModelPass {
    public:
        void Init();
        void Shutdown();

        void BeginScene(const glm::mat4& viewProj, const glm::vec3& viewPos, const Reference<LightingBuffer>& lighting);
        void EndScene();

        void SetShadowMapData(const glm::mat4& lightSpaceMatrix, const Reference<ShadowMap>& shadowMap, float shadowIntensity);

        void SubmitModel(const Reference<AnimatedModel>& model, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
        void SubmitModel(const Reference<AnimatedModel>& model, const glm::vec3& position, const glm::vec3& size = glm::vec3(1.0f), const glm::vec4& color = glm::vec4(1.0f), const glm::vec3& rotation = glm::vec3(0.0f));

        void ClearBatches();

        struct Stats
        {
            uint32_t DrawCalls = 0;
            uint32_t Instances = 0;
            uint32_t Triangles = 0;
        };
        const Stats& GetStats() const { return m_Stats; }
        void ResetStats() { m_Stats = {}; }

    private:
        void Flush();
        void RegisterMesh(const AnimatedMesh* mesh);
        void PruneStaleBatches();

    private:
        struct InstanceData
        {
            glm::mat4 Transform;
            glm::vec4 Color;
        };

        struct AnimatedBatchKey
        {
            const AnimatedMesh* MeshPtr = nullptr;
            const AnimatedModel* ModelPtr = nullptr;

            bool operator==(const AnimatedBatchKey& other) const {
                return MeshPtr == other.MeshPtr && ModelPtr == other.ModelPtr;
            }
        };

        struct AnimatedBatchKeyHash
        {
            std::size_t operator()(const AnimatedBatchKey& key) const {
                std::size_t h1 = std::hash<const void*>{}(key.MeshPtr);
                std::size_t h2 = std::hash<const void*>{}(key.ModelPtr);
                return h1 ^ (h2 << 1);
            }
        };

        struct AnimatedMeshBatch
        {
            AnimatedBatchKey Key;
            std::weak_ptr<void> LifetimeToken;
            std::vector<InstanceData> InstancesData;
            bool WarnedOverflow = false;
        };

        static constexpr uint32_t MaxInstances = 10000;
        static constexpr uint32_t DefaultBatchReserve = 64;
        static constexpr uint32_t ShadowMapTextureSlot = 3;

        std::unordered_map<AnimatedBatchKey, AnimatedMeshBatch, AnimatedBatchKeyHash> m_MeshBatches;
        std::vector<AnimatedMeshBatch*> m_FlushList;

        Reference<VertexBuffer> m_InstanceVBO;
        Reference<Shader> m_Shader;
        Reference<Texture2D> m_WhiteTexture;

        glm::mat4 m_ViewProjection;
        Stats m_Stats;
    };

}