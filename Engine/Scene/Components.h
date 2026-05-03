#pragma once

#include <glm/glm.hpp>


namespace Monsi {

	struct MeshComponent {
		// TODO for 3D renderer
		MeshComponent() = default;
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

}