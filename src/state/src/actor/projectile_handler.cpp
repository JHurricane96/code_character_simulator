#include "actor/projectile_handler.h"
#include <algorithm>

namespace state {

ProjectileHandler::ProjectileHandler(
	int64_t next_arrow_id
	) :
	next_arrow_id(next_arrow_id),
	arrows() {}

void ProjectileHandler::CreateArrow(
	PlayerId player_id,
	physics::Vector2D start,
	Actor* attack_target,
	int64_t max_speed,
	int64_t ttl,
	int64_t size,
	int64_t damage
) {
	std::shared_ptr<Arrow> arrow(
		new Arrow(
			next_arrow_id,
			player_id,
			damage,
			max_speed,
			ttl,
			size,
			start,
			attack_target));
	arrows.push_back(arrow);
	next_arrow_id++;
}

void ProjectileHandler::HandleActors(
	std::vector<std::vector<std::shared_ptr<Tower> > >& towers,
	Terrain* terrain
) {
	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		for (auto tower : towers[pid]) {
			if (tower->IsReadyToAttack()) {
				auto target = tower->GetAttackTarget();
				auto damage = tower->GetAttack();
				auto tower_pos = tower->GetPosition();
				auto arrow_dest = target->GetPosition();
				terrain->CoordinateToTerrainElement(tower_pos).GetTerrainType();
				auto tot_damage = damage *
				  DamageMultiplier [terrain->CoordinateToTerrainElement(tower_pos)
										.GetTerrainType()]
								   [terrain->CoordinateToTerrainElement(arrow_dest)
								   		.GetTerrainType()];
				CreateArrow(
					tower->GetPlayerId(),
					tower_pos,
					target,
					tower->GetArrowSpeed(),
					tower->GetArrowTtl(),
					tower->GetArrowSize(),
					tot_damage);
			}
		}
	}
}

void ProjectileHandler::HandleArrows(
	int64_t delta_time
) {
	std::vector<std::shared_ptr<Arrow> > new_arrows;
	for (int64_t i = 0; i < arrows.size(); i++) {
		arrows[i]->Update(delta_time);
		if (!arrows[i]->IsDone()) {
			new_arrows.push_back(arrows[i]);
		}
	}
	arrows = new_arrows;
}

void ProjectileHandler::Update(
	int64_t delta_time,
	std::vector<std::vector<std::shared_ptr<Tower> > >& towers,
	Terrain* terrain
) {
	HandleActors(towers, terrain);
	HandleArrows(delta_time);
}

}
