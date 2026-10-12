#pragma once

#include "Components.h"
#include "ScriptableEntity.h"
#include "TimeStep.h"
#include "Input.h"
#include "MonsiKeyCodes.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Monsi {

	class PlayerControllerScript : public ScriptableEntity
	{
	public:
		void OnUpdate(TimeStep ts) override
		{
			auto& transform = GetComponent<TransformComponent>();

			float velocity = m_MoveSpeed * ts;

			glm::vec3 forward = glm::rotate(transform.Rotation, glm::vec3(0.0f, 0.0f, -1.0f));
			glm::vec3 right = glm::rotate(transform.Rotation, glm::vec3(1.0f, 0.0f, 0.0f));

			if (Input::KeyPressed(MONSI_KEY_W)) {
				transform.Translation += forward * velocity;
			}
			if (Input::KeyPressed(MONSI_KEY_S)) {
				transform.Translation -= forward * velocity;
			}
			if (Input::KeyPressed(MONSI_KEY_A)) {
				transform.Translation -= right * velocity;
			}
			if (Input::KeyPressed(MONSI_KEY_D)) {
				transform.Translation += right * velocity;
			}
		}

		void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
		float GetMoveSpeed() const { return m_MoveSpeed; }

	private:
		float m_MoveSpeed = 10.0f;
	};

}