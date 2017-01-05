#include <algorithm>
#include "state.h"

namespace state {

void SetIfValid(int * a, int value) {
	if (a) {
		* a = value;
	}
}

State::State() : terrain(1), path_planner(1) {}

State::State(
		Terrain terrain,
		std::vector<std::shared_ptr<Actor> > actors
	):
	terrain(terrain),
	path_planner(terrain.GetRows()),
	actors(actors) {}

State::State(
		Terrain terrain,
		std::vector<std::vector<std::shared_ptr<Actor> > > sorted_actors,
		std::vector<std::shared_ptr<King> > kings,
		std::vector<std::shared_ptr<Base> > bases,
		std::vector<std::shared_ptr<Flag> > flags
	):
	terrain(terrain),
	sorted_actors(sorted_actors),
	path_planner(terrain.GetRows()),
	kings(kings),
	bases(bases),
	flags(flags) {
		for (int64_t i = 0; i <= LAST_PLAYER; i++) {
			list_act_id_t l;
			for (auto actor: sorted_actors[i])
				l.push_back(actor->GetId());
			player_unit_ids.push_back(l);
		}
		actors = flatten(sorted_actors);
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
	terrain(terrain),
	path_planner(terrain.GetRows()),
	actors(actors),
	kings(kings),
	bases(bases),
	flags(flags) {}

void State::MoveUnits(
	list_act_id_t unit_ids,
	physics::Vector2D destination,
	std::shared_ptr<FormationMaker> formation_maker,
	std::vector<int64_t> terrain_weights
) {
	std::vector<std::shared_ptr<Actor> > units;
	for (auto unit_id : unit_ids) {
		units.push_back(actors[unit_id]);
	}

	path_planner.MakeFormation(
		units[0]->GetPlayerId(),
		units,
		terrain,
		formation_maker,
		destination,
		terrain_weights
	);
}

std::vector<std::shared_ptr<Tower> > State::GetTowers(
	PlayerId player_id
) {
	return towers[player_id];
}

std::vector<std::shared_ptr<Tower> > State::GetEnemyTowers(
	PlayerId player_id
) {
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

std::shared_ptr<King> State::GetEnemyKing(PlayerId player_id) {
}

std::shared_ptr<Base> State::GetBase(PlayerId player_id) {
	return bases[player_id];
}

std::shared_ptr<Base> State::GetEnemyBase(PlayerId player_id) {
	return bases[(player_id + 1) % (LAST_PLAYER + 1)];
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

const Terrain& State::GetTerrain() const {
	return terrain;
}

list_act_id_t State::GetActorEnemies(PlayerId player_id, act_id_t actor_id) {
	list_act_id_t enemies;
	int64_t size = terrain.CoordinateToTerrainElement(physics::Vector2D(0,0))
						  .GetSize();
	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		if (pid != player_id)
			for (auto id: player_unit_ids[pid]) {
				if (actors[id]->GetPosition().distance(actors[actor_id]->GetPosition())
					 < actors[actor_id]->GetLosRadius()*size)
					enemies.push_back(id);
			}
	}
	return enemies;
}

list_act_id_t State::GetPlayerEnemyIds(PlayerId player_id) {
	list_act_id_t all_enemies;
	for (int pid = 0; pid <= LAST_PLAYER; pid++)
			if (pid != player_id) {
				for (auto actor: sorted_actors[pid]) {
					auto te = terrain.CoordinateToTerrainElement(actor->GetPosition());
					if (te.GetLos(player_id) == DIRECT_LOS)
						all_enemies.push_back(actor->GetId());
				}
			}
	return all_enemies;
}

void State::Update(int64_t delta_time) {
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
		actor->Update(delta_time);
	}

	terrain.Update(sorted_actors);
}

}
