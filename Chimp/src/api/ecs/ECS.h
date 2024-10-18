#pragma once

#include "stdafx.h"

namespace Chimp {

#ifdef CHIMP_FLECS
	typedef flecs::entity EntityId;
	class ECS {
	public:
		ECS() = default;

		template <typename... Components>
		using ComponentsRefFunc = std::function<void(Components&...)>;

		// Create an entity with a list of components
		// Components - Types of components the entity should have
		// func - This function will be called once the entity is created, it should set data of components to their default values
		template <typename... Components>
		EntityId CreateEntity(const ComponentsRefFunc<Components...> &func) {
			return m_World.entity().insert(func);
		}

		// Get all entities with all of a set of components
		// Components - Types of components the entity should have
		// func - This function will be called once for every entity with all the specified components
		template <typename... Components>
		void GetEntitiesWithComponents(const ComponentsRefFunc<Components...>& func) {
			m_World.system<Components>().each(func);
		}

	private:
		flecs::world m_World;
	};
#endif
}