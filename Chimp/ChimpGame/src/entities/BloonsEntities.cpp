#include "BloonsEntities.h"

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type)
	{
		auto& mesh = engine.GetResourceManager().GetSprites().Get(GAME_SRC + std::string("/assets/textures/")+ Bloons::TexturePaths[(size_t)type]);
		Chimp::EntityId entity = CreateBaseEntity(ecs, mesh, {
			{ position.x, position.y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 100, 100, 1.0f }
			});

		ecs.SetComponent(entity, Chimp::HealthComponent{ Bloons::HealthValues[(size_t)type]});
		ecs.SetComponent(entity, MoveableComponent{ Bloons::SpeedValues[(size_t)type]});
		ecs.SetComponent(entity, BloonComponent{});

		return entity;
	}
}

void Bloons::DamageBloon(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::EntityId bloonEntity, int damage)
{
	auto health = ecs.GetMutableComponent<Chimp::HealthComponent>(bloonEntity);
	health->Health -= damage;

	// Update texture
	const auto bloonType = HealthToBloonType(health->Health);
	auto& newMesh = engine.GetResourceManager().GetSprites().Get(GAME_SRC + std::string("/assets/textures/") + Bloons::TexturePaths[(size_t)bloonType]);
	ecs.GetMutableComponent<Chimp::MeshComponent>(bloonEntity)->Mesh = &newMesh;
}
