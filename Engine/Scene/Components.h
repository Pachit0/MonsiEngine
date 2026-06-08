#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "Mesh.h"
#include "ModelLoader.h"

namespace Monsi {
// 
// 	struct MeshComponent {
// 
// 
// 		MeshComponent() = default;
// 		MeshComponent(const MeshComponent& other) = default;
// 	};
// 
// 	struct ModelComponent {
// 		Reference<Model> ModelAsset;
// 
// 		ModelComponent() = default;
// 		ModelComponent(const ModelComponent& other) = default;
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