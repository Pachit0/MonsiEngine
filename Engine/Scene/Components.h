#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "CubePass.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "ScriptableEntity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Monsi {

	struct MeshComponent {
		Reference<Mesh>  MeshAsset;

		MeshComponent() = default;
		MeshComponent(const MeshComponent& other) = default;
		MeshComponent(const Reference<Mesh>& mesh) : MeshAsset(mesh) {}
	};

	struct ModelComponent {
		Reference<Model> ModelAsset;

		ModelComponent() = default;
		ModelComponent(const ModelComponent& other) = default;
		ModelComponent(const Reference<Model>& model) : ModelAsset(model) {}
	};

	struct MaterialComponent {
		Reference<Texture2D> Texture;
		glm::vec4 TintColor = glm::vec4(1.0f);

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent& other) = default;
	};

	struct LightComponent {
		enum class LightType { Directional = 0, Point = 1 };

		LightType Type = LightType::Point;
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		float Radius = 10.0f;
		glm::vec3 Direction = { 0.0f, -1.0f, 0.0f };

		LightComponent() = default;
		LightComponent(const LightComponent& other) = default;
	};

// 	struct TransformComponent {
// 		glm::vec3 Translation = glm::vec3{ 0.0f,0.0f,0.0f };
// 		glm::vec3 Rotation = glm::vec3{ 0.0f,0.0f,0.0f };
// 		glm::vec3 Scale = glm::vec3{ 1.0,1.0f,1.0f };
// 
// 		TransformComponent() = default;
// 		TransformComponent(const TransformComponent& other) = default;
// 		TransformComponent(const glm::vec3& translation) : Translation(translation) {}
// 
// 		glm::mat4 GetTransform() const {
// 			glm::mat4 rotation = 
// 			  glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
// 			* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 })
// 			* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });
// 
// 			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
// 		}
// 	};

	struct TransformComponent {
		glm::vec3 Translation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::mat4_cast(Rotation)
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 Color = glm::vec4({ 1.0f,1.0f,1.0f,1.0f });

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

	};

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateFuncPtr)();
		void (*DestroyInstanceFuncPtr)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateFuncPtr = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceFuncPtr = [](NativeScriptComponent* nativeScript) { delete nativeScript->Instance; nativeScript->Instance = nullptr; };
		}
	};
}