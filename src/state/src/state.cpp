#include <algorithm>
#include "state.h"

namespace state {

void SetIfValid(int * a, int value) {
	if (a) {
		* a = value;
	}
}

State::State()
	: projectile_handler(actors.size()),
	path_planner(1),
	terrain(1) {}

State::State(
		Terrain terrain,
		std::vector<std::shared_ptr<Actor> > actors
	):
	actors(actors),
	projectile_handler(actors.size()),
	path_planner(terrain.GetRows()),
	terrain(terrain),
	flag_capture_score(std::vector<int64_t>(LAST_PLAYER+1, 0)),
	base_poisoning_penalty(std::vector<int64_t>(LAST_PLAYER+1, 0)) {}

State::State(
		Terrain terrain,
		std::vector<std::vector<std::shared_ptr<Actor> > > sorted_actors,
		std::vector<std::shared_ptr<King> > kings,
		std::vector<std::shared_ptr<Base> > bases,
		std::vector<std::shared_ptr<Flag> > flags,
		std::vector<std::vector<std::shared_ptr<Tower> > > towers,
		std::vector<std::vector<std::shared_ptr<Scout> > > scouts,
		std::vector<std::vector<std::shared_ptr<Magician> > > magicians,
		std::vector<std::vector<std::shared_ptr<Swordsman> > > swordsmen
	):
	sorted_actors(sorted_actors),
	towers(towers),
	magicians(magicians),
	swordsmen(swordsmen),
	scouts(scouts),
	flags(flags),
	kings(kings),
	bases(bases),
	projectile_handler(),
	path_planner(terrain.GetRows()),
	terrain(terrain),
	flag_capture_score(std::vector<int64_t>(LAST_PLAYER+1, 0)),
	base_poisoning_penalty(std::vector<int64_t>(LAST_PLAYER+1, 0)) {
		for (int64_t i = 0; i <= LAST_PLAYER; i++) {
			list_act_id_t l;
			for (auto actor: sorted_actors[i])
				l.push_back(actor->GetId());
			player_unit_ids.push_back(l);
		}
		actors = flatten(sorted_actors);
		projectile_handler = ProjectileHandler(actors.size());
		std::sort(actors.begin(), actors.end(),
			[]( const std::shared_ptr<Actor>& a1,
				const std::shared_ptr<Actor>& a2) {
					return a1->GetId() < a2->GetId();
				}
			);
	}


State::State(
		Terrain terrain,
		std::vector<std::shared_ptr<Actor> > actors,
		std::vector<std::shared_ptr<King> > kings,
		std::vector<std::shared_ptr<Base> > bases,
		std::vector<std::shared_ptr<Flag> > flags
	):
	actors(actors),
	flags(flags),
	kings(kings),
	bases(bases),
	projectile_handler(actors.size()),
	path_planner(terrain.GetRows()),
	terrain(terrain),
	flag_capture_score(std::vector<int64_t>(LAST_PLAYER+1, 0)),
	base_poisoning_penalty(std::vector<int64_t>(LAST_PLAYER+1, 0)) {}

std::shared_ptr<Actor> State::GetActorFromId(
		PlayerId player_id,
		act_id_t actor_id,
		int * success
) {
	if (actor_id < 0 || actor_id >= actors.size()) {
		SetIfValid(success, 0);
		return nullptr;
	}

	if (actors[actor_id]->GetPlayerId() != player_id) {
		SetIfValid(success, -1);
		return nullptr;
	}

	SetIfValid(success, 1);
	return actors[actor_id];
}

std::vector<std::shared_ptr<Actor> > State::GetPlayerActors(
	PlayerId player_id
) {
	return sorted_actors[player_id];
}

void State::MoveUnits(
	PlayerId player_id,
	list_act_id_t unit_ids,
	physics::Vector2D destination,
	std::shared_ptr<FormationMaker> formation_maker,
	std::vector<int64_t> terrain_weights,
	std::vector<physics::Vector2D> &path,
	int * success
) {
	if (unit_ids.empty()) {
		SetIfValid(success, 0);
		return;
	}

	for (auto act_id : unit_ids) {
		if (act_id >= actors.size() || act_id < 0) {
			SetIfValid(success, -1);
			return;
		}
		if (actors[act_id]->GetPlayerId() != player_id) {
			SetIfValid(success, -2);
			return;
		}
		if (actors[act_id]->IsDead()) {
			SetIfValid(success, -3);
			return;
		}
	}

	auto bounds = terrain.GetSize();
	if (destination.x < 0 || destination.y < 0 ||
		destination.x >= bounds.x || destination.y >= bounds.y ) {
		SetIfValid(success, -4);
		return;
		}

	if (!IsValidFormation(formation_maker.get(), unit_ids.size())) {
		SetIfValid(success, -5);
		return;
	}

	if (terrain_weights.size() != 3) {
		SetIfValid(success, -6);
		return;
	}

	for (auto weight : terrain_weights) {
		if (weight <= 0) {
			SetIfValid(success, -7);
			return;
		}
	}

	SetIfValid(success, 1);

	std::vector<std::shared_ptr<Actor> > units;
	for (auto unit_id : unit_ids) {
		units.push_back(actors[unit_id]);
	}

	path.clear();
	path_planner.MakeFormation(
		units[0]->GetPlayerId(),
		units,
		terrain,
		formation_maker,
		destination,
		terrain_weights,
		path
	);
}

void State::MoveUnits(
	PlayerId player_id,
	list_act_id_t unit_ids,
	std::vector<physics::Vector2D> destinations,
	std::shared_ptr<FormationMaker> formation_maker,
	int * success
) {
	if (unit_ids.empty()) {
		SetIfValid(success, 0);
		return;
	}

	for (auto act_id : unit_ids) {
		if (act_id >= actors.size() || act_id < 0) {
			SetIfValid(success, -1);
			return;
		}
		if (actors[act_id]->GetPlayerId() != player_id) {
			SetIfValid(success, -2);
			return;
		}
		if (actors[act_id]->IsDead()) {
			SetIfValid(success, -3);
			return;
		}
	}

	if (destinations.empty()) {
		SetIfValid(success, -4);
		return;
	}

	auto bounds = terrain.GetSize();
	for (auto destination : destinations) {
		if (destination.x < 0 || destination.y < 0 ||
			destination.x >= bounds.x || destination.y >= bounds.y ) {
			SetIfValid(success, -5);
			return;
		}
	}

	if (!IsValidFormation(formation_maker.get(), unit_ids.size())) {
		SetIfValid(success, -6);
		return;
	}

	SetIfValid(success, 1);

	std::vector<std::shared_ptr<Actor> > units;
	for (auto unit_id : unit_ids) {
		units.push_back(actors[unit_id]);
	}

	path_planner.MakeFormation(
		units[0]->GetPlayerId(),
		units,
		formation_maker,
		destinations
	);
}

list_act_id_t State::GetPlayerUnitIds(PlayerId player_id) {
	return player_unit_ids[(int)player_id];
}

std::vector<std::shared_ptr<Magician> > State::GetMagicians(
	PlayerId player_id
) {
	return magicians[player_id];
}

std::vector<std::shared_ptr<Scout> > State::GetScouts(PlayerId player_id) {
	return scouts[player_id];
}

std::vector<std::shared_ptr<Scout> > State::GetEnemyScouts(
	PlayerId player_id
) {
	auto grid_element_size =
		terrain.CoordinateToTerrainElement(physics::Vector2D(0,0)).GetSize();
	auto enemy_scouts = scouts[(player_id + 1) % (LAST_PLAYER + 1)];
	std::vector<std::shared_ptr<Scout> > visible_enemy_scouts;
	for (auto scout : enemy_scouts) {
		if (terrain.CoordinateToTerrainElement(scout->GetPosition())
			       .GetLos(player_id) == DIRECT_LOS ) {
			if (scout->GetPosition().distance(bases[player_id]->GetPosition())
				<= bases[player_id]->GetLosRadius() * grid_element_size) {
				visible_enemy_scouts.push_back(scout);
			}
			else {
				for (auto tower : towers[player_id]) {
					if (scout->GetPosition().distance(tower->GetPosition())
						<= tower->GetLosRadius() * grid_element_size) {
						visible_enemy_scouts.push_back(scout);
						break;
					}
				}
			}
		}
	}
	return visible_enemy_scouts;
}

std::vector<std::shared_ptr<Magician> > State::GetEnemyMagicians(
	PlayerId player_id
) {
	auto enemy_magicians = magicians[(player_id + 1) % (LAST_PLAYER + 1)];
	std::vector<std::shared_ptr<Magician> > visible_enemy_magicians;
	for (auto magician : enemy_magicians) {
		if (terrain.CoordinateToTerrainElement(magician->GetPosition())
		   .GetLos(player_id) == DIRECT_LOS)
			visible_enemy_magicians.push_back(magician);
	}
	return visible_enemy_magicians;
}

std::vector<std::shared_ptr<Swordsman> > State::GetSwordsmen(
	PlayerId player_id
) {
	return swordsmen[player_id];
}

std::vector<std::shared_ptr<Swordsman> > State::GetEnemySwordsmen(
	PlayerId player_id
) {
	auto enemy_swordsmen = swordsmen[(player_id + 1) % (LAST_PLAYER + 1)];
	std::vector<std::shared_ptr<Swordsman> > visible_enemy_swordsmen;
	for (auto swordsman : enemy_swordsmen) {
		if (terrain.CoordinateToTerrainElement(swordsman->GetPosition())
		   .GetLos(player_id) == DIRECT_LOS)
			visible_enemy_swordsmen.push_back(swordsman);
	}
	return visible_enemy_swordsmen;
}

std::vector<std::shared_ptr<Tower> > State::GetTowers(
	PlayerId player_id
) {
	return towers[player_id];
}

std::vector<std::shared_ptr<Tower> > State::GetEnemyTowers(
	PlayerId player_id
) {
	auto enemy_towers = towers[(player_id + 1) % (LAST_PLAYER + 1)];
	std::vector<std::shared_ptr<Tower> > visible_enemy_towers;
	for (auto tower : enemy_towers) {
		if (terrain.CoordinateToTerrainElement(tower->GetPosition())
		   .GetLos(player_id) == DIRECT_LOS)
			visible_enemy_towers.push_back(tower);
	}
	return visible_enemy_towers;
}

std::shared_ptr<Flag> State::GetFlag(PlayerId player_id) {
	return flags[player_id];
}

std::shared_ptr<Flag> State::GetEnemyFlag(PlayerId player_id) {
	return flags[(player_id + 1) % (LAST_PLAYER + 1)];
}

std::shared_ptr<King> State::GetKing(PlayerId player_id) {
	return kings[player_id];
}

std::shared_ptr<King> State::GetEnemyKing(
	PlayerId player_id,
	int * success
) {
	auto enemy_king = kings[(player_id + 1) % (LAST_PLAYER + 1)];
	if (terrain.CoordinateToTerrainElement(enemy_king->GetPosition())
			   .GetLos(player_id) == DIRECT_LOS) {
		SetIfValid(success, 1);
		return enemy_king;
	}
	SetIfValid(success, 0);
	return nullptr;
}

std::shared_ptr<Base> State::GetBase(PlayerId player_id) {
	return bases[player_id];
}

std::shared_ptr<Base> State::GetEnemyBase(PlayerId player_id) {
	return bases[(player_id + 1) % (LAST_PLAYER + 1)];
}

std::vector<std::shared_ptr<FireBall> > State::GetProjectiles() {
	return projectile_handler.GetProjectiles();
}


list_act_id_t State::GetRespawnables(PlayerId player_id) {
	list_act_id_t respawnables;

	for (auto actor : sorted_actors[player_id]) {
		if (actor->IsDead() && actor->GetTimeToRespawn() <= 0) {
			respawnables.push_back(actor->GetId());
		}
	}

	return respawnables;
}

void State::AttackUnit(
	PlayerId player_id,
	list_act_id_t attacker_ids,
	act_id_t attack_target_id,
	int * success
) {
	if (attacker_ids.empty()) {
		SetIfValid(success, 0);
		return;
	}

	for (auto act_id : attacker_ids) {
		if (act_id >= actors.size() || act_id < 0) {
			SetIfValid(success, -1);
			return;
		}
		if (actors[act_id]->GetPlayerId() != player_id) {
			SetIfValid(success, -2);
			return;
		}
		if (actors[act_id]->IsDead()) {
			SetIfValid(success, -3);
		}
	}

	if (attack_target_id >= actors.size() || attack_target_id < 0) {
		SetIfValid(success, -4);
		return;
	}

	auto target = actors[attack_target_id];

	if (target->GetPlayerId() == player_id) {
		SetIfValid(success, -5);
		return;
	}
	if (target->IsDead()) {
		SetIfValid(success, -6);
		return;
	}
	if (terrain
		.CoordinateToTerrainElement(target->GetPosition())
		.GetLos(player_id) != DIRECT_LOS) {
		SetIfValid(success, -7);
		return;
	}

	for (int64_t i = 0; i < attacker_ids.size(); ++i) {
		actors[attacker_ids[i]]->AttackUnit(target.get());
	}

	SetIfValid(success, 1);
}

void State::FlagCapture(PlayerId player_id, int * success) {
	auto king = GetKing(player_id);
	auto enemy_flag = GetEnemyFlag(player_id);

	if (king->IsDead()) {
		SetIfValid(success, 0);
	}

	if (king->GetPosition().distance(enemy_flag->GetPosition()) >
		king->GetSize() + enemy_flag->GetSize()) {
		SetIfValid(success, -1);
		return;
	}

	if (king->HasFlag()) {
		SetIfValid(success, -2);
		return;
	}

	king->CaptureFlag(enemy_flag.get());
	enemy_flag->Capture(king.get());
	SetIfValid(success, 1);

}

void State::FlagDrop(PlayerId player_id, int * success) {
	auto king = GetKing(player_id);
	auto enemy_flag = GetEnemyFlag(player_id);
	auto base = GetBase(player_id);

	if (king->IsDead()) {
		SetIfValid(success, 0);
	}

	if (king->GetPosition().distance(base->GetPosition()) >
		king->GetSize() + base->GetSize()) {
		SetIfValid(success, -1);
		return;
	}

	if (!king->HasFlag()) {
		SetIfValid(success, -2);
		return;
	}

	king->DropFlag();
	enemy_flag->Drop();
	SetIfValid(success, 1);
}

float State::PlanPath(
	physics::Vector2D start,
	physics::Vector2D destination,
	std::vector<int64_t> weights,
	std::vector<physics::Vector2D> &path,
	int * success
) {
	auto bounds = terrain.GetSize();

	if (start.x < 0 || start.y < 0 ||
		start.x >= bounds.x || start.y >= bounds.y ) {
		SetIfValid(success, 0);
		return -1;
	}

	if (destination.x < 0 || destination.y < 0 ||
		destination.x >= bounds.x || destination.y >= bounds.y ) {
		SetIfValid(success, -1);
		return -1;
	}

	if (weights.size() != 3) {
		SetIfValid(success, -2);
		return -1;
	}

	for (auto weight : weights) {
		if (weight <= 0) {
			SetIfValid(success, -3);
			return -1;
		}
	}

	SetIfValid(success, 1);

	path.clear();
	return path_planner.PlanPath(
		start,
		destination,
		terrain,
		path,
		weights
	);
}

void State::RespawnUnit(
	PlayerId player_id,
	act_id_t actor_id,
	act_id_t respawn_location,
	int * success
) {
	if (actor_id < 0 || actor_id >= actors.size()) {
		SetIfValid(success, 0);
		return;
	}

	auto actor = actors[actor_id];
	if (actor->GetPlayerId() != player_id) {
		SetIfValid(success, -1);
		return;
	}

	if (!actor->IsDead()) {
		SetIfValid(success, -2);
		return;
	}

	if (actor->GetTimeToRespawn() > 0) {
		SetIfValid(success, -3);
		return;
	}

	if (respawn_location < 0 || respawn_location >= actors.size()) {
		SetIfValid(success, -4);
		return;
	}

	auto respawn_actor = actors[respawn_location];
	if (respawn_actor->GetPlayerId() != player_id) {
		SetIfValid(success, -5);
		return;
	}

	if (respawn_actor->GetActorType() != ActorType::TOWER &&
		respawn_actor->GetActorType() != ActorType::BASE) {
		SetIfValid(success, -6);
		return;
	}

	actor->SetRespawnLocation(respawn_actor->GetPosition());
	SetIfValid(success, 1);
}

bool CompareActorsByXCoordinate(
	std::shared_ptr<Actor> a,
	std::shared_ptr<Actor> b
) {
	return (a->GetPosition().x < b->GetPosition().x);
}

Terrain State::GetTerrain() {
	return terrain;
}

list_act_id_t State::GetActorEnemies(PlayerId player_id, act_id_t actor_id) {
	list_act_id_t enemies;
	int64_t size = terrain.CoordinateToTerrainElement(physics::Vector2D(0,0))
						  .GetSize();
	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		if (pid != player_id)
			for (auto id: player_unit_ids[pid]) {
				if(actors[actor_id]->GetActorType() != ActorType::TOWER &&
					actors[actor_id]->GetActorType() != ActorType::BASE &&
					actors[id]->GetActorType() == ActorType::SCOUT) {}
				else if (actors[id]->GetPosition().distance(actors[actor_id]->GetPosition())
					 < actors[actor_id]->GetLosRadius()*size)
					enemies.push_back(id);
			}
	}
	return enemies;
}

list_act_id_t State::GetPlayerEnemyIds(PlayerId player_id) {
	list_act_id_t all_enemies;
	auto grid_size = terrain.CoordinateToTerrainElement(physics::Vector2D(0,0))
	                        .GetSize();
	for (int pid = 0; pid <= LAST_PLAYER; pid++)
		if (pid != player_id) {
			for (auto actor : sorted_actors[pid]) {
				auto te = terrain.CoordinateToTerrainElement(actor->GetPosition());
				if (te.GetLos(player_id) == DIRECT_LOS &&
					actor->GetActorType() != ActorType::SCOUT)
						all_enemies.push_back(actor->GetId());
				}
			}
	for (auto scout : GetEnemyScouts(player_id))
		all_enemies.push_back(scout->GetId());
	return all_enemies;
}

void State::Update(float delta_time) {
	for (auto units : sorted_actors) {
		std::sort(
			units.begin(),
			units.end(),
			CompareActorsByXCoordinate
		);
	}

	path_planner.Update(sorted_actors);

	for (auto actor : actors) {
		auto target = actor->GetAttackTarget();
		if (target != nullptr) {
			if (terrain
				.CoordinateToTerrainElement(target->GetPosition())
				.GetLos(actor->GetPlayerId()) != DIRECT_LOS) {
				actor->StopAttack();
			}
		}

		if (actor->GetActorType() == ActorType::TOWER) {
			std::shared_ptr<Tower> tower = std::static_pointer_cast<Tower>(actor);
			if (tower->IsDead()) {
				auto pid = tower->GetTowerOwner();
				if (pid != TowerOwner::UNOWNED) {
					towers[(int)pid].erase( std::find(
										towers[(int)pid].begin(),
										towers[(int)pid].end(),
										tower) );
					sorted_actors[(int)pid].erase( std::find(
										sorted_actors[(int)pid].begin(),
										sorted_actors[(int)pid].end(),
										actor) );
				}
				if (tower->Contend(delta_time, sorted_actors)) {
					auto id = actor->GetPlayerId();
					sorted_actors[id].push_back(actor);
					towers[id].push_back(tower);
				}
			}
		}
		actor->Update(delta_time);
		actor->CheckBounds(terrain.GetSize());
	}

	for (int64_t i = 0; i <= LAST_PLAYER; i++) {
		base_poisoning_penalty[i] += bases[i]->GetBasePoisonPenalty(sorted_actors[i]);
	}

	terrain.Update(sorted_actors);

	projectile_handler.Update(delta_time, towers, magicians, &terrain);

}

void State::MergeWithBuffer(const State& state, PlayerId player_id) {
	for (int64_t i = 0; i < actors.size(); ++i) {
		if (actors[i]->GetPlayerId() == player_id) {
			actors[i]->MergeWithBuffer(state.actors[i].get(), actors);
		}
	}

	path_planner.MergeWithBuffer(
		state.path_planner,
		player_id,
		actors
	);

	kings[player_id]->MergeWithBuffer(
		state.kings[player_id].get(), actors
	);

	flags[(player_id + 1) % (LAST_PLAYER + 1)]->MergeWithBuffer(
		state.flags[(player_id + 1) % (LAST_PLAYER + 1)].get(), actors
	);
}

void State::MergeWithMain(const State& state) {
	for (int64_t i = 0; i < actors.size(); ++i) {
		actors[i]->MergeWithMain(state.actors[i].get(), actors);
	}

	for (int64_t i = 0; i <= LAST_PLAYER; ++i) {
		kings[i]->MergeWithMain(state.kings[i].get(), actors);
		flags[i]->MergeWithMain(state.flags[i].get(), actors);

		towers[i].clear();
		for (auto tower : state.towers[i]) {
			towers[i].push_back(
				std::static_pointer_cast<Tower>(actors[tower->GetId()])
			);
			towers[i].back()->MergeWithMain(tower.get());
		}

		sorted_actors[i].clear();
		for (auto actor : state.sorted_actors[i]) {
			sorted_actors[i].push_back(actors[actor->GetId()]);
		}
	}

	path_planner.MergeWithMain(state.path_planner, actors);
	terrain.MergeWithMain(state.terrain);
	projectile_handler.MergeWithMain(state.projectile_handler, actors);

}

}
