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

float GetEnemyAllyHpRatio (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	int k
)	{
		state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);

		std::vector<state::MagicianView> allyMagicians = state->GetMagicians();
		std::vector<state::SwordsmanView> allySwordsmen = state->GetSwordsmen();
		std::vector<state::TowerView> allyTowers = state->GetTowers();

		std::vector<state::EnemyMagicianView> enemyMagicians = state->GetEnemyMagicians();
		std::vector<state::EnemySwordsmanView> enemySwordsmen = state->GetEnemySwordsmen();
		std::vector<state::EnemyTowerView> enemyTowers = state->GetEnemyTowers();

		std::vector<std::pair<int64_t, float>> allyPairs;
		std::vector<std::pair<int64_t, float>> enemyPairs;

		for (auto mage : allyMagicians) {
			allyPairs.push_back (
				std::make_pair (
					mage.GetHp(),
					mage.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}
		for (auto swordsman : allySwordsmen) {
			allyPairs.push_back (
				std::make_pair (
					swordsman.GetHp(),
					swordsman.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}
		for (auto tower : allyTowers) {
			allyPairs.push_back (
				std::make_pair (
					tower.GetHp(),
					tower.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}
		for (auto enemyMage : enemyMagicians) {
			enemyPairs.push_back (
				std::make_pair (
					enemyMage.GetHp(),
					enemyMage.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}
		for (auto enemySwordsman : enemySwordsmen) {
			enemyPairs.push_back (
				std::make_pair (
					enemySwordsman.GetHp(),
					enemySwordsman.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}
		for (auto enemyTower : enemyTowers) {
			enemyPairs.push_back (
				std::make_pair (
					enemyTower.GetHp(),
					enemyTower.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}

		std::sort(allyPairs.begin(), allyPairs.end(), SortedBySecondElement);
		std::sort(enemyPairs.begin(), enemyPairs.end(), SortedBySecondElement);

		float allyHp = 0, enemyHp = 0;
		int allyCount = 0, enemyCount = 0;

		if(allyPairs.size() + enemyPairs.size() < k)
			k = allyPairs.size() + enemyPairs.size();

		for (int i = 0; i < k; i++) {
			if (allyCount < allyPairs.size() && enemyCount < enemyPairs.size()) {
				if (enemyPairs[i].second < allyPairs[i].second) {
					enemyHp += enemyPairs[i].first;
					enemyCount++;
				}
				else {
					allyHp += allyPairs[i].first;
					allyCount++;
				}
			}
			else if (allyCount < allyPairs.size()) {
				allyHp += allyPairs[i].first;
				allyCount++;
			}
			else if (enemyCount < enemyPairs.size()) {
				enemyHp += enemyPairs[i].first;
				enemyCount++;
			}
		}
		if(enemyCount == 0)
			return -1;
		if (allyHp == 0)
			allyHp++;

		return (enemyHp / allyHp);
}

}