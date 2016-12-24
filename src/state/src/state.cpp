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

bool CompareActorsByXCoordinate(
	std::shared_ptr<Actor> a,
	std::shared_ptr<Actor> b
) {
	return (a->GetPosition().x < b->GetPosition().x);
}

const Terrain& State::GetTerrain() const {
	return terrain;
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
}

}
