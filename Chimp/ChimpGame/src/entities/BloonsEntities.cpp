#include "BloonsEntities.h"

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type)
	{
		Chimp::Mesh& mesh = engine.GetAssetManager().GetMesh(Bloons::BloonIds[(size_t)type]);
		Chimp::EntityId entity = CreateBaseEntity(ecs, mesh, {
			{ position.x, position.y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 100, 100, 1.0f }
			});

		return entity;
	}
}