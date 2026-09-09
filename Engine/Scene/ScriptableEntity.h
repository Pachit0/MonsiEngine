#pragma once
#include "Entity.h"

namespace Monsi {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		void Init(Entity entity)
		{
			m_Entity = entity;
			OnCreate();
		}
		
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(TimeStep timeStep) {}

	private:
		Entity m_Entity;
		friend class Scene;
		friend class SceneHierarchyUnit;
	};

}