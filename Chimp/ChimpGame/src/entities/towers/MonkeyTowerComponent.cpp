#include "MonkeyTowerComponent.h"
#include "entities/BaseEntity.h"
#include "entities/towers/TowerTypes.h"

Chimp::EntityId Entities::CreateMonkeyTower(Chimp::ECS& ecs, Chimp::Mesh& mesh, Chimp::Vector2f position)
{
	constexpr float scale = 45;
	Chimp::EntityId ent = Entities::CreateBaseEntity(ecs, mesh, Chimp::Transform(
		{ position.x, position.y, TOWER_Z },
		{ 0, 0.8 ,6 },
		{ scale, scale, scale }
	));

	ecs.SetComponent(ent, MonkeyTowerComponent{});
	ecs.SetComponent(ent, TowerComponent{ TowerType::MONKEY });

	return ent;
}
