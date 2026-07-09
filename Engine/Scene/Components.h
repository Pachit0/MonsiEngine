#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "CubePass.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "ScriptableEntity.h"

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

	struct TransformComponent {
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator const glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
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