#include "MonsiPch.h"
#include "Entity.h"
#include "Components.h"
#include <MonsiMath.h>
#include <glm/glm.hpp>

namespace Monsi {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_Handle(handle), m_Scene(scene)
	{
	}

	void Entity::SetParent(Entity parent, bool keepWorldTransform)
	{
		if (m_Scene == nullptr || !parent || parent.m_Handle == m_Handle)
			return;

		entt::entity current = parent.m_Handle;
		while (current != entt::null)
		{
			if (current == m_Handle)
				return;

			if (!m_Scene->m_Registry.all_of<RelationshipComponent>(current))
				break;

			current = m_Scene->m_Registry.get<RelationshipComponent>(current).Parent;
		}

		glm::mat4 worldTransform = m_Scene->GetWorldTransform(m_Handle);

		RemoveParent();

		auto& relationship = m_Scene->m_Registry.get_or_emplace<RelationshipComponent>(m_Handle);
		relationship.Parent = parent.m_Handle;

		auto& parentRelationship = m_Scene->m_Registry.get_or_emplace<RelationshipComponent>(parent.m_Handle);
		parentRelationship.Children.push_back(m_Handle);

		if (keepWorldTransform && HasComponent<TransformComponent>())
		{
			glm::mat4 localTransform = glm::inverse(m_Scene->GetWorldTransform(parent.m_Handle)) * worldTransform;

			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(localTransform, translation, rotation, scale);

			auto& transform = GetComponent<TransformComponent>();
			transform.Translation = translation;
			transform.Rotation = glm::quat(rotation);
			transform.Scale = scale;
		}
	}

	void Entity::RemoveParent(bool keepWorldTransform)
	{
		if (m_Scene == nullptr || !m_Scene->m_Registry.any_of<RelationshipComponent>(m_Handle))
			return;

		glm::mat4 worldTransform(1.0f);
		if (keepWorldTransform)
			worldTransform = m_Scene->GetWorldTransform(m_Handle);

		auto& relationship = m_Scene->m_Registry.get<RelationshipComponent>(m_Handle);
		if (relationship.Parent == entt::null)
			return;

		if (m_Scene->m_Registry.valid(relationship.Parent) && m_Scene->m_Registry.all_of<RelationshipComponent>(relationship.Parent))
		{
			auto& parentRelationship = m_Scene->m_Registry.get<RelationshipComponent>(relationship.Parent);
			auto& children = parentRelationship.Children;
			children.erase(std::remove(children.begin(), children.end(), m_Handle), children.end());
		}
		relationship.Parent = entt::null;

		if (keepWorldTransform && HasComponent<TransformComponent>())
		{
			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(worldTransform, translation, rotation, scale);

			auto& transform = GetComponent<TransformComponent>();
			transform.Translation = translation;
			transform.Rotation = glm::quat(rotation);
			transform.Scale = scale;
		}
	}

}