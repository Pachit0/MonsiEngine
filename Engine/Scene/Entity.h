#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Monsi {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default; // explicitly showing that we have a copy constructor

		template<typename Component>
		bool HasComponent() {
			return m_Scene->m_Registry.any_of<Component>(m_Handle);
		}

		template<typename Component, typename... Args>
		Component& AddComponent(Args&&... args) {
			ENGINE_ASSERT(!HasComponent<Component>(), "The component is already in this entity!");

			return m_Scene->m_Registry.emplace<Component>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename Component>
		Component& GetComponent() {
			ENGINE_ASSERT(HasComponent<Component>(), "The component is not in this entity!");
			return m_Scene->m_Registry.get<Component>(m_Handle);
		}

		template<typename Component>
		void RemoveComponent() {
			ENGINE_ASSERT(HasComponent<Component>(), "The component is not in this entity!");

			m_Scene->m_Registry.remove<Component>(m_Handle);
		}

		operator bool() const { return m_Handle != entt::null; }
		operator uint32_t() { return (uint32_t)m_Handle; }
		bool operator==(const Entity& other) const { return m_Handle == other.m_Handle && m_Scene == other.m_Scene;	}
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_Handle{ entt::null };
		Scene* m_Scene = nullptr;
	};
	

}