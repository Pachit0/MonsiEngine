#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "Mesh.h"
#include "ModelLoader.h"

namespace Monsi {

// 	struct MeshComponent {
// 		enum class Primitive { None = 0, Cube, Sphere, Capsule, Quad };
// 
// 		Reference<Model> ModelAsset;
// 		Primitive Type = Primitive::None;
// 
// 		MeshComponent() = default;
// 		MeshComponent(const MeshComponent& other) = default;
// 		MeshComponent(const std::string& filepath) : ModelAsset(CreateReference<Model>(filepath)) {}
// 		MeshComponent(Primitive primitiveType) : Type(primitiveType) {}
// 	};
// 
// 	struct MaterialComponent {
// 		Reference<Texture2D> Texture;
// 		glm::vec4 TintColor = glm::vec4(1.0f);
// 
// 		MaterialComponent() = default;
// 		MaterialComponent(const MaterialComponent& other) = default;
// 		MaterialComponent(const Reference<Texture2D>& texture) : Texture(texture) {}
// 	};
// 
// 	struct LightComponent {
// 		glm::vec3 Direction = { 0.0f, -1.0f, 0.0f };
// 		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
// 		float Intensity = 1.0f;
// 
// 		LightComponent() = default;
// 		LightComponent(const LightComponent& other) = default;
// 	};

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
}