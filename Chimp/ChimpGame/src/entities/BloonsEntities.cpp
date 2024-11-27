#include "BloonsEntities.h"

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type)
	{
		Chimp::Mesh& mesh = engine.GetResourceManager().GetMeshStorage().GetMesh(Bloons::BloonIds[(size_t)type]);
		Chimp::EntityId entity = CreateBaseEntity(ecs, mesh, {
			{ position.x, position.y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 100, 100, 1.0f }
			});

		ecs.SetComponent<HealthComponent>(entity, HealthComponent{ Bloons::HealthValues[(size_t)type]});
		ecs.SetComponent<MoveableComponent>(entity, MoveableComponent{ Bloons::SpeedValues[(size_t)type]});
		ecs.SetComponent<BloonComponent>(entity, BloonComponent{});

		return entity;
	}
}

void Bloons::DamageBloon(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::EntityId bloonEntity, int damage)
{
	auto health = ecs.GetMutableComponent<HealthComponent>(bloonEntity);
	health->Health -= damage;

	// Update texture
	const auto bloonType = HealthToBloonType(health->Health);
	auto& newMesh = engine.GetResourceManager().GetMeshStorage().GetMesh(BloonIds[(size_t)bloonType]);
	ecs.GetMutableComponent<Chimp::MeshComponent>(bloonEntity)->Mesh = &newMesh;
}
