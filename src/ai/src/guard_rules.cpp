/**
 * @file guard_rules.h
 * Implementation file of the Guard Strategies and Utilities for a Single Actor Unit per tick
 */

#include "state.h"
#include "utilities.h"
#include "player_state_handler/player_state_handler.h"
#include "guard_rules.h"

#include <bits/stdc++.h>

using namespace std;

namespace ai {

	class DefaultFormation : public state::FormationMaker {
	std::vector<physics::Vector2D> ReturnFormation(int64_t formation_size) override {
		return std::vector<physics::Vector2D>(formation_size, physics::Vector2D(0, 0));
	}
};

/**
 * Comparison function for the Sort function used in GetEnemyAllyNumRatio
 *
 * @param[in]  a     Comparison parameter 1
 * @param[in]  b     Comparison parameter 2
 *
 * @return     boolean var indicating whether a is lesser than b
 */
bool SortBySecondElement2(const pair<state::act_id_t, float> &a, const pair<state::act_id_t, float> &b) {
	return (a.second < b.second);
}

/**
 * Function to find the ratio of HP of Enemies to Ally from a given Unit's k neighbours
 *
 * @param[in]  state  The state
 * @param[in]  id     The identifier for the Unit
 * @param[in]  k      Number of neighbours to consider
 *
 * @return     The enemy ally HP ratio.
 */
float GetEnemyAllyNumRatio(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id, int k) {
	state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
	state::list_act_id_t allies = state->GetPlayerUnitIds();
	state::list_act_id_t enemies = state->GetPlayerEnemyIds();
	vector<pair<state::act_id_t, float>> allypairs;
	vector<pair<state::act_id_t, float>> enemypairs;
	for (int i = 0; i < allies.size(); i++) {
		state::UnitView allyUnit = state->GetUnitFromId(allies[i], nullptr);
		allypairs.push_back(make_pair(allies[i], allyUnit.GetPosition().distance(parentUnit.GetPosition())));
	}
	for (int i = 0; i < enemies.size(); i++) {
		state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
		enemypairs.push_back(make_pair(enemies[i], enemyUnit.GetPosition().distance(parentUnit.GetPosition())));
	}
	std::sort(allypairs.begin(), allypairs.end(), SortBySecondElement2);
	std::sort(enemypairs.begin(), enemypairs.end(), SortBySecondElement2);
	float allyCount = 0;
	float enemyCount = 0;
	for (int i = 0, ai = 0, ei = 0; i < k; i++) {
		if (ai < allypairs.size() && ei < enemypairs.size()) {
			if (enemypairs[i].second < allypairs[i].second) {
				enemyCount += state->GetEnemyUnitFromId(enemypairs[i].first, nullptr).GetHp();
				ei++;
			}
			else {
				allyCount += state -> GetUnitFromId(allypairs[i].first, nullptr).GetHp();
				ai++;
			}
		}
		else if (ai < allypairs.size()) {
			allyCount += state -> GetUnitFromId(allypairs[i].first, nullptr).GetHp();
			ai++;
		}
		else if (ei < enemypairs.size()) {
			enemyCount += state->GetEnemyUnitFromId(enemypairs[i].first, nullptr).GetHp();
			ei++;
		}
	}
	if (allyCount == 0) allyCount++;
	return enemyCount / allyCount;
}

/**
 * Fuction to find Nearest Enemy from a given Unit
 *
 * @param[in]  state  The state
 * @param[in]  id     The identifier for the Unit
 *
 * @return     A pair object containing (NearestEnemyUnitId, NearestEnemyUnitDistance)
 */
pair<state::act_id_t, float> NearestEnemy(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id) {
	state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
	state::list_act_id_t enemies = state->GetPlayerEnemyIds();
	state::act_id_t nearestEnemyId = -1;
	float nearestEnemyDistance = -1, dist;
	for (int i = 0; i < enemies.size(); i++) {
		state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
		dist = enemyUnit.GetPosition().distance(parentUnit.GetPosition());
		if (nearestEnemyDistance == -1) {
			nearestEnemyDistance = dist;
			nearestEnemyId = enemyUnit.GetId();
		}
		else if (dist < nearestEnemyDistance) {
			nearestEnemyDistance = dist;
			nearestEnemyId = enemyUnit.GetId();
		}
	}
	return make_pair(nearestEnemyId, nearestEnemyDistance);
}

/**
 * Function to speculate whether a given Unit is defending tower or not
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 * @param[in]  nearestEnemyDistance  The nearest enemy distance from the Unit
 *
 * @return     True if defending tower, False otherwise.
 */
bool IsDefendingTower(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id, float nearestEnemyDistance) {
	state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
	std::vector<state::TowerView> allyTowers = state->GetTowers();
	float nearestTowerDistance = -1, dist;
	for (int i = 0; i < allyTowers.size(); i++) {
		dist = allyTowers[i].GetPosition().distance(parentUnit.GetPosition());
		if (nearestTowerDistance == -1) {
			nearestTowerDistance = dist;
		}
		else if (dist < nearestTowerDistance) {
			nearestTowerDistance = dist;
		}
	}
	
	if (nearestEnemyDistance <= parentUnit.GetAttackRange() && nearestTowerDistance == 0) return true;
	else return false;
}

/**
 * Function to speculate whether a given Unit is defending king or not
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 * @param[in]  nearestEnemyDistance  The nearest enemy distance from the Unit
 *
 * @return     True if defending king, False otherwise.
 */
bool IsDefendingKing(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id, float nearestEnemyDistance) {
	state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
	float kingDistance = state->GetKing().GetPosition().distance(parentUnit.GetPosition());
	if (nearestEnemyDistance <= parentUnit.GetAttackRange() && kingDistance == 0) return true;
	else return false;
}

/**
 * Function to speculate whether a given Unit is defending base or not.
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 * @param[in]  nearestEnemyDistance  The nearest enemy distance from the Unit
 *
 * @return     True if defending base, False otherwise.
 */
bool IsDefendingBase(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id, float nearestEnemyDistance) {
	state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
	float baseDistance = state->GetBase().GetPosition().distance(parentUnit.GetPosition());
	if (nearestEnemyDistance <= parentUnit.GetAttackRange() && baseDistance == 0) return true;
	else return false;
}

// /**
//  * Class for guard rules for guard strategies and utilities
//  */
// class GuardRules {
// 	/**
// 	 * The given state of the game
// 	 */
// 	std::shared_ptr<state::PlayerStateHandler> state;
// 	/**
// 	 * The formation generally used for defense
// 	 */
// 	state::FormationMaker *formation;
// public:
// 	GuardRules(std::shared_ptr<state::PlayerStateHandler> playerStateHandler, state::FormationMaker *formationMaker);
// 	*
// 	 * The strategy for the defense per unit per tick
// 	 *
// 	 * @param[in]  unitId  The unit identifier

// 	void Strategy(state::act_id_t unitId);
// 	/**
// 	 * The utility function or the transition specifier for the Guard state
// 	 *
// 	 * @param[in]  unitId  The unit identifier
// 	 *
// 	 * @return     int var (1-GoToAtack, 2-GoToRetreat, 3-GoToExplore, 4-GoToGuard)
// 	 */
// 	int Utility(state::act_id_t unitId);
// };

// GuardRules::GuardRules(std::shared_ptr<state::PlayerStateHandler> playerStateHandler, state::FormationMaker *formationMaker) {
// 	state = playerStateHandler;
// 	formation = formationMaker;
// }

GuardRules::GuardRules() {
	formation = new DefaultFormation();
}

void GuardRules::Strategy(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state) {
	if (state->GetUnitFromId(unitId, nullptr).GetVelocity().magnitude() > 0 || state->GetUnitFromId(unitId, nullptr).GetPathPlannerHelper().IsPathPlanning()) {
		return;
	}
	vector<int64_t> terrain_weights(3);
	if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::MAGICIAN) {
		terrain_weights[state::MOUNTAIN] = 1;
		terrain_weights[state::PLAIN] = 2;
		terrain_weights[state::FOREST] = 4;
	}
	else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SWORDSMAN) {
		terrain_weights[state::MOUNTAIN] = 2;
		terrain_weights[state::PLAIN] = 2;
		terrain_weights[state::FOREST] = 1;
	}
	else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::KING) {
		terrain_weights[state::MOUNTAIN] = 1;
		terrain_weights[state::PLAIN] = 1;
		terrain_weights[state::FOREST] = 4;
	}
	else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SCOUT) {
		terrain_weights[state::MOUNTAIN] = 1;
		terrain_weights[state::PLAIN] = 1;
		terrain_weights[state::FOREST] = 1;
	}
	if (state->GetFlag().GetVelocity().magnitude() > 0 || GetEnemyAllyNumRatio(state, state->GetFlag().GetId(), 20) > 1) {
		pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetFlag().GetId());
		if (state->GetUnitFromId(nearestEnemy.first, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, nullptr);
		}
		else {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetFlag().GetPosition(), formation, terrain_weights, tempPath, nullptr);
		}
	}
	else if (GetEnemyAllyNumRatio(state, state->GetKing().GetId(), 20) > 1) {
		pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetKing().GetId());
		if (state->GetUnitFromId(nearestEnemy.first, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, nullptr);
		}
		else {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetKing().GetPosition(), formation, terrain_weights, tempPath, nullptr);
		}
	}
	else if (GetEnemyAllyNumRatio(state, state->GetBase().GetId(), 20) > 1) {
		if (state->GetBase().GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) == 0) {
			pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetBase().GetId());
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, nullptr);
		}
		else {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetBase().GetPosition(), formation, terrain_weights, tempPath, nullptr);
		}
	}
	else {
		state::act_id_t weakestTowerId = -1;
		float weakestTowerHp = FLT_MAX, hp;
		std::vector<state::TowerView> towers = state->GetTowers();
		for (int i = 0; i < towers.size(); i++) {
			hp = towers[i].GetHp();
			if (hp < weakestTowerHp) {
				weakestTowerHp = hp;
				weakestTowerId = towers[i].GetId();
			}
		}
		if (state->GetUnitFromId(weakestTowerId, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) == 0) {
			pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, weakestTowerId);
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, nullptr);
		}
		else {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetUnitFromId(weakestTowerId, nullptr).GetPosition(), formation, terrain_weights, tempPath, nullptr);
		}
		
	} 
}

int GuardRules::Utility(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state) {
	float enemyAllyHpRatio = GetEnemyAllyNumRatio(state, unitId, 20);
	pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, unitId);
	bool isDefendingKing = IsDefendingKing(state, unitId, nearestEnemy.second);
	bool isDefendingTower = IsDefendingTower(state, unitId, nearestEnemy.second);
	bool isDefendingBase = IsDefendingBase(state, unitId, nearestEnemy.second);
	if (enemyAllyHpRatio == 0) {
		return 3;
	}
	else if (isDefendingKing || isDefendingTower || isDefendingBase) {
		return 4;
	}
	else if (enemyAllyHpRatio <= 1) {
		return 1;
	}
	else if (enemyAllyHpRatio > 1 && !isDefendingBase && !isDefendingTower && !isDefendingKing) {
		return 2;
	}
}

}

