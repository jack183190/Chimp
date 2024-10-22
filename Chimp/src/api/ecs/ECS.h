#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"

namespace Chimp {

#ifdef CHIMP_FLECS
	typedef flecs::entity EntityId;
	class ECS {
	public:
		ECS() = default;

	public:
		// A view represents a set of entities each with the same common set of components
		template <typename... Components>
		class View {
			friend class ECS;
		private:
			using ComponentTuple = std::tuple<Components&...>;
			View(flecs::world& world) {
				world.system<Components...>().each([this](Components&... components) {
					m_Components.emplace_back(components...);
					});
				world.progress();
			}

		public:
			using iterator = typename std::vector<ComponentTuple>::iterator;

			iterator begin() { return m_Components.begin(); }
			iterator end() { return m_Components.end(); }

		private:
			std::vector<ComponentTuple> m_Components;
		};

	public:

		// Create an entity with no components
		EntityId CreateEntity() {
			return m_World.entity();
		}

		// Set a component on an entity
		// Component - The component type to set
		// entity - The entity to set the component on
		// component - The value to set the component to
		template <typename Component>
		void SetComponent(EntityId entity, const Component& component) {
			entity.set(component);
		}

		// Get a component from an entity
		// entity - The entity to get the component from
		template <typename Component>
		ConstOptionalReference<Component> GetComponent(EntityId entity) {
			return ConstOptionalReference<Component>(entity.get<Component>());
		}

		// Get a component from an entity
		template <typename Component>
		OptionalReference<Component> GetMutableComponent(EntityId entity) {
			return OptionalReference<Component>(entity.get_mut<Component>());
		}

		// Get all entities with all of a set of components
		// Components - Types of components the entity should have
		// func - This function will be called once for every entity with all the specified components
		template <typename... Components>
		View<Components...> GetEntitiesWithComponents() {
			// This is slower than if we just passed a lambda in instead of returning a view
			// but it's only about 20-25% difference in release mode and imo the api is much nicer.
			return View<Components...>(m_World);
		}

	private:
		flecs::world m_World;
	};
#endif
}