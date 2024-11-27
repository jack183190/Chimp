#pragma once

#include "stdafx.h"
#include "EntityIdComponent.h"

namespace Entities {
	inline Chimp::EntityId CreateBaseEntity(Chimp::ECS& ecs, Chimp::Mesh& mesh, Chimp::Transform transform = {}) {
		Chimp::EntityId entity = ecs.CreateEntity();

		ecs.SetComponent(entity, Chimp::TransformComponent{ transform });
		ecs.SetComponent(entity, Chimp::MeshComponent{ &mesh });
		ecs.SetComponent<EntityIdComponent>(entity, EntityIdComponent{ entity });

		return entity;
	}
}