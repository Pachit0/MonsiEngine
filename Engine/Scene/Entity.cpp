#include "MonsiPch.h"
#include "Entity.h"

namespace Monsi {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_Handle(handle), m_Scene(scene)
	{
	}

}