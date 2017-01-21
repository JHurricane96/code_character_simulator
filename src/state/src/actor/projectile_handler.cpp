#include "actor/projectile_handler.h"
#include <algorithm>

namespace state {

ProjectileHandler::ProjectileHandler() {}

ProjectileHandler::ProjectileHandler(
	int64_t next_arrow_id
	) :
	arrows(),
	next_arrow_id(next_arrow_id) {}

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


std::vector<std::shared_ptr<Arrow> >
ProjectileHandler::GetProjectiles() {
	return arrows;
}

template <typename ActorType>
void ProjectileHandler::HandleActors(
	std::vector<std::vector<std::shared_ptr<ActorType> > >& actors,
	Terrain* terrain
) {
	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		for (auto actor : actors[pid]) {
			if (actor->IsReadyToAttack()) {
				auto target = actor->GetAttackTarget();
				auto damage = actor->GetAttack();
				auto actor_pos = actor->GetPosition();
				auto arrow_dest = target->GetPosition();
				terrain->CoordinateToTerrainElement(actor_pos).GetTerrainType();
				auto tot_damage = damage *
				  DamageMultiplier [terrain->CoordinateToTerrainElement(actor_pos)
										.GetTerrainType()]
								   [terrain->CoordinateToTerrainElement(arrow_dest)
								   		.GetTerrainType()];
				CreateArrow(
					actor->GetPlayerId(),
					actor_pos,
					target,
					actor->GetArrowSpeed(),
					actor->GetArrowTtl(),
					actor->GetArrowSize(),
					tot_damage);
				actor->SetReadyToAttackToFalse();
			}
		}
	}
}

void ProjectileHandler::HandleArrows(
	float delta_time
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
	float delta_time,
	std::vector<std::vector<std::shared_ptr<Tower> > >& towers,
	std::vector<std::vector<std::shared_ptr<Archer> > >& archers,
	Terrain* terrain
) {
	HandleActors<Tower>(towers, terrain);
	HandleActors<Archer>(archers, terrain);
	HandleArrows(delta_time);
}

void ProjectileHandler::MergeWithMain(
	const ProjectileHandler& proj_handler,
	std::vector<std::shared_ptr<Actor> > actors
) {
	arrows.clear();
	for (auto arrow : proj_handler.arrows) {
		arrows.push_back(std::shared_ptr<Arrow>(new Arrow(*arrow)));
		arrows.back()->MergeWithMain(arrow.get(), actors);
	}
	next_arrow_id = proj_handler.next_arrow_id;
}

}
