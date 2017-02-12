#include <vector>
#include <map>
#include <cstdlib>
#include <functional>
#include <set>
#include <algorithm>
#include <cmath>
#include <memory>
#include "ai.h"
#include "group_state.h"
#include "attack_rules.h"
#include "guard_rules.h"
#include "ai_utilities.h"

namespace ai {

AI::AI() {
	init_groups = false;
}

void AI::SetSortedEnemies (
	std::shared_ptr<state::PlayerStateHandler> state
) {

	auto enemies = state->GetPlayerEnemyIds();
	for(auto enemy : enemies) {
		sortedEnemies.push_back (
				std::make_pair (
					enemy,
					state->GetEnemyUnitFromId(enemy, nullptr).GetHp()
				)
			);
	}
	std::sort(sortedEnemies.begin(), sortedEnemies.end(), SortedBySecondElement);
	return;
}

state::act_id_t AI::GetOptimalRespawnLocation (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t to_respawn_id
) {
	std::vector<state::TowerView> towers = state->GetTowers();
	if(towers.size()==0) {
		return state->GetBase().GetId();
	}
	else {
		state::act_id_t towerId;
		float leastDistance = FLT_MAX;
		for(auto tower : towers) {

			if(leastDistance > tower.GetPosition().distance(state->GetEnemyFlag().GetPosition())) {

				leastDistance = tower.GetPosition().distance(state->GetEnemyFlag().GetPosition());
				towerId = tower.GetId();
			}
		}
		return towerId;
	}

}

void AI::Update(std::shared_ptr<state::PlayerStateHandler> state) {
	if (!init_groups){
		for (auto actid : state -> GetPlayerUnitIds()) {
			groups.push_back(new Group(actid));
		}
		init_groups = true;
	}

	AI::SetSortedEnemies(state);

	auto to_respawn_ids = state->GetRespawnables();

	for (auto to_respawn_id : to_respawn_ids) {
		if(state->GetUnitFromId(to_respawn_id, nullptr).GetActorType() != state::ActorType::KING) {
			auto locationId = GetOptimalRespawnLocation(state, to_respawn_id);
			state->RespawnUnit(to_respawn_id, locationId, NULL);
		}
		if (rand() % 2 == 0) {
			state->RespawnUnit(to_respawn_id, state->GetBase().GetId(), NULL);
		}
		else {
			auto towers = state->GetTowers();
			int chosen = rand() % towers.size();
			state->RespawnUnit(to_respawn_id, towers[chosen].GetId(), NULL);
		}
	}
	
	for (auto &group : groups)
		group -> update(state, sortedEnemies, kingsGuard, flagsGuard);
}

}
