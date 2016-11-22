#include "player_state_handler/player_state_handler.h"
#include <bits/stdc++.h>

namespace ai {

bool SortedBySecondElement (
	const std::pair<state::act_id_t,float> &a,
	const std::pair<state::act_id_t,float> &b
) 	{
		return (a.second < b.second);
}

float relu (float x) {
	if (x > 0)
		return x;
	return 0;
}

float GetEnemyAllyHpRatioByThreshold(
		std::shared_ptr<state::PlayerStateHandler> state,
		state::act_id_t id,
		float threshold
	) {
	state::list_act_id_t allies = state->GetPlayerUnitIds(), enemies = state->GetPlayerEnemyIds();
	float allySum = 0, enemySum = 0;
	for (int i = 0; i < allies.size(); i++){
		if (state->GetUnitFromId(allies[i], nullptr).GetPosition().distance(state->GetUnitFromId(id, nullptr).GetPosition()) <= threshold) {
			allySum += state->GetUnitFromId(allies[i], nullptr).GetHp();
		}
	}
	for (int i = 0; i < enemies.size(); i++){
		if (state->GetUnitFromId(enemies[i], nullptr).GetPosition().distance(state->GetUnitFromId(id, nullptr).GetPosition()) <= threshold) {
			enemySum += state->GetEnemyUnitFromId(enemies[i], nullptr).GetHp();
		}
	}
	if (allySum == 0) allySum = 1;
	return enemySum/allySum;
}

}