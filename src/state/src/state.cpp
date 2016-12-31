#include <algorithm>
#include "state.h"

namespace state {

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
	actors(flatten(sorted_actors)),
	path_planner(terrain.GetRows()),
	kings(kings),
	bases(bases),
	flags(flags) {
		list_act_id_t l;
		for (int64_t i = 0; i <= LAST_PLAYER; i++) {
			for (auto actor: sorted_actors[i])
				l.push_back(actor->GetId());
			player_unit_ids.push_back(l);
		}
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

void State::FlagCapture(PlayerId player_id, int * success) {
	auto king = GetKing(player_id);
	auto enemy_flag = GetEnemyFlag(player_id);

	if (king->GetPosition().distance(enemy_flag->GetPosition()) >
		king->GetSize() + enemy_flag->GetSize()) {
		if (success) {
			*success = 0;
		}
		return;
	}

	if (king->HasFlag()) {
		if (success) {
			*success = -1;
		}
		return;
	}

	king->CaptureFlag(enemy_flag.get());
	enemy_flag->Capture(king.get());
	if (success) {
		*success = 1;
	}

}

void State::FlagDrop(PlayerId player_id, int * success) {
	auto king = GetKing(player_id);
	auto enemy_flag = GetEnemyFlag(player_id);
	auto base = GetBase(player_id);

	if (king->GetPosition().distance(base->GetPosition()) >
		king->GetSize() + base->GetSize()) {
		if (success) {
			*success = 0;
		}
		return;
	}

	if (!king->HasFlag()) {
		if (success) {
			*success = -1;
		}
		return;
	}

	king->DropFlag();
	enemy_flag->Drop();
	if (success) {
		*success = 1;
	}
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

	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		if (pid != player_id)
			for (auto id: player_unit_ids[pid]) {
                if (actors[id]->GetPosition().distance(actors[actor_id]->GetPosition())
                     < actors[actor_id]->GetLosRadius())
					enemies.push_back(id);
			}
	}
	return enemies;
}

list_act_id_t State::GetPlayerEnemyIds(PlayerId player_id) {
	list_act_id_t all_enemies;
	for(int64_t i = 0; i < terrain.GetRows(); ++i)
		for(int64_t j = 0; j < terrain.GetRows(); ++j)
			for (int pid = 0; pid <= LAST_PLAYER; pid++)
					if (pid != player_id) {
						for (auto id: terrain.OffsetToTerrainElement(
								physics::Vector2D(i,j)).GetUnits((PlayerId)pid)) {
							all_enemies.push_back(id);
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
		actor->Update(delta_time);
	}

	terrain.Update(sorted_actors);
}

}
