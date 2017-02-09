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

bool InAttackRange (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId,
	state::EnemyUnitView enemy
)	{
	auto unit = state -> GetUnitFromId(unitId, nullptr);
	return (enemy.GetPosition().distance(unit.GetPosition()) <= unit.GetAttackRange());
}

state::act_id_t GetOptimalTarget (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	float threshold
) {
	auto enemies = sortedEnemies;

	/** 
	 * Check if enemy king is available, 
	 * in range and return it if it needs to be attacked 
	 */
	std::unique_ptr<int> success(new int());
	auto enemy_king = state -> GetEnemyKing(success.get());
	if (*success == 1 
		&& InAttackRange(state, unitId, enemy_king) 
		&& (GetEnemyAllyHpRatio( state, enemy_king.GetId(), threshold) > 1)
	) {
		return enemy_king.GetId();
	}
	
	/* If king not available, get the next weakest enemy */
	else {
		for(auto enemy : sortedEnemies) {
			if (InAttackRange(state, unitId, state->GetEnemyUnitFromId(enemy.first, nullptr))
				&& (GetEnemyAllyHpRatio( state, enemy.first, threshold) > 1)
			) {
				return enemy.first;
			}	
		}
	}
	return -1;
}

float GetDistanceFromUnit(
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId, physics::Vector2D position
)	{
		return position.distance(state -> GetUnitFromId(unitId, nullptr).GetPosition());
}

std::pair<state::act_id_t, float> NearestEnemy (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	state::ActorType* type = nullptr
)	{

		state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
		state::list_act_id_t enemies = state->GetPlayerEnemyIds();

		state::act_id_t nearestEnemyId = -1;
		float nearestEnemyDistance = FLT_MAX, dist;

		if(type == nullptr) {

			for (int i = 0; i < enemies.size(); i++) {

				state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
				dist = enemyUnit.GetPosition().distance(parentUnit.GetPosition());

				if (dist < nearestEnemyDistance) {
					nearestEnemyDistance = dist;
					nearestEnemyId = enemyUnit.GetId();
				}
			}
		}
		else {
			for (int i = 0; i < enemies.size(); i++) {

				state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
				dist = enemyUnit.GetPosition().distance(parentUnit.GetPosition());

				if (dist < nearestEnemyDistance && (enemyUnit.GetActorType() == *type) ) {
					nearestEnemyDistance = dist;
					nearestEnemyId = enemyUnit.GetId();
				}
			}
		}
		return std::make_pair(nearestEnemyId, nearestEnemyDistance);
}

state::act_id_t NearestEnemyByType (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	std::vector<state::ActorType> types
)	{

		state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
		state::list_act_id_t enemies = state->GetPlayerEnemyIds();

		state::act_id_t nearestEnemyId = -1;
		float nearestEnemyDistance = FLT_MAX, dist;

		for (int i = 0; i < enemies.size(); i++) {

			state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
			dist = enemyUnit.GetPosition().distance(parentUnit.GetPosition());

			bool isTypeValid = false;
			for (int j = 0; j < types.size();j++){
				if (enemyUnit.GetActorType() == types[j]){
					isTypeValid = true;
					break;
				}
			}
			if (dist < nearestEnemyDistance && isTypeValid ) {
				nearestEnemyDistance = dist;
				nearestEnemyId = enemyUnit.GetId();
			}
		}
		return nearestEnemyId;
}

std::pair<state::act_id_t, int> WeakestEnemy (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	float threshhold
)	{

		state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
		state::list_act_id_t enemies = state->GetPlayerEnemyIds();

		state::act_id_t weakestEnemyId = -1;
		int weakestEnemyHP = INT_MAX, hp;
		float dist;

		for (int i = 0; i < enemies.size(); i++) {

			state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
			hp = enemyUnit.GetHp();
			dist = enemyUnit.GetPosition().distance(parentUnit.GetPosition());

			if (hp < weakestEnemyHP && dist < threshhold) {
				weakestEnemyHP = hp;
				weakestEnemyId = enemyUnit.GetId();
			}
		}
		return std::make_pair(weakestEnemyId, weakestEnemyHP);
}

bool IsAttackingEnemy (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id
)	{
		return (state -> GetUnitFromId(id, nullptr).GetAttackTarget(nullptr) != nullptr);
}

bool IsAttackingKing (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id
)	{
		auto target = state->GetUnitFromId(id, nullptr).GetAttackTarget(nullptr);
		if (target == nullptr)
			return false;
		if (target->GetActorType() == state::ActorType::KING) {
			return true;
		}
		return false;
}

bool IsEnemyTowerDominating(std::shared_ptr<state::PlayerStateHandler> state) {
	std::vector<state::TowerView> allyTowers = state->GetTowers();
	std::vector<state::EnemyTowerView> foeTowers = state->GetEnemyTowers();
	float allySum = 0, foeSum = 0;
	for (int i = 0; i < allyTowers.size(); i++) {
		allySum += allyTowers[i].GetHp();	
	}
	for (int i = 0; i < foeTowers.size(); i++) {
		foeSum += foeTowers[i].GetHp();	
	}
	if (foeSum > allySum) return true;
	else return false;
}

}
