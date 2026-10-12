#pragma once

#include "Components.h"
#include "ScriptableEntity.h"
#include "TimeStep.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Monsi {

	class CameraFollowScript : public ScriptableEntity
	{
	public:
		void OnUpdate(TimeStep ts) override
		{
			if (!m_HasTarget)
				return;

			auto& camTransform = GetComponent<TransformComponent>();
			auto& targetTransform = m_TargetEntity.GetComponent<TransformComponent>();

			float dt = ts;

			float posT = 1.0f - glm::exp(-m_PositionSmoothSpeed * dt);
			float rotT = 1.0f - glm::exp(-m_RotationSmoothSpeed * dt);

			glm::vec3 desiredPosition = targetTransform.Translation + (targetTransform.Rotation * m_PositionOffset);
			camTransform.Translation = glm::mix(camTransform.Translation, desiredPosition, posT);

			glm::quat desiredRotation;
			if (m_LookAtTarget)
			{
				glm::vec3 lookTarget = targetTransform.Translation + m_LookAtOffset;
				glm::vec3 direction = glm::normalize(lookTarget - camTransform.Translation);

				if (glm::length2(direction) > 0.0001f)
				{
					desiredRotation = glm::quatLookAt(direction, glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					desiredRotation = camTransform.Rotation;
				}
			}
			else
			{
				desiredRotation = targetTransform.Rotation * m_RotationOffset;
			}

			camTransform.Rotation = glm::slerp(camTransform.Rotation, desiredRotation, rotT);
		}

		void SetTarget(Entity target)
		{
			m_TargetEntity = target;
			m_HasTarget = (bool)target;
		}

		Entity GetTarget() const { return m_TargetEntity; }

		void SetPositionOffset(const glm::vec3& offset) { m_PositionOffset = offset; }
		void SetRotationOffset(const glm::quat& rotOffset) { m_RotationOffset = rotOffset; }

		const glm::vec3& GetPositionOffset() const { return m_PositionOffset; }
		const glm::quat& GetRotationOffset() const { return m_RotationOffset; }

		void SetSmoothSpeeds(float posSpeed, float rotSpeed)
		{
			m_PositionSmoothSpeed = posSpeed;
			m_RotationSmoothSpeed = rotSpeed;
		}

		void SetPositionSmoothSpeed(float speed) { m_PositionSmoothSpeed = speed; }
		float GetPositionSmoothSpeed() const { return m_PositionSmoothSpeed; }

		void SetRotationSmoothSpeed(float speed) { m_RotationSmoothSpeed = speed; }
		float GetRotationSmoothSpeed() const { return m_RotationSmoothSpeed; }

		void SetLookAtTarget(bool lookAt) { m_LookAtTarget = lookAt; }
		bool GetLookAtTarget() const { return m_LookAtTarget; }

		void SetLookAtOffset(const glm::vec3& offset) { m_LookAtOffset = offset; }
		const glm::vec3& GetLookAtOffset() const { return m_LookAtOffset; }

	private:
		Entity m_TargetEntity;
		bool m_HasTarget = false;

		glm::vec3 m_PositionOffset = { 0.0f, 3.0f, 6.0f };
		glm::quat m_RotationOffset = glm::quat(glm::radians(glm::vec3(-25.0f, 0.0f, 0.0f)));

		float m_PositionSmoothSpeed = 10.0f;
		float m_RotationSmoothSpeed = 10.0f;

		bool m_LookAtTarget = true;
		glm::vec3 m_LookAtOffset = { 0.0f, 1.0f, 0.0f };
	};

}