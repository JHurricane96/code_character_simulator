/**
 * @file guard_rules.h
 * Implementation file of the Guard Strategies and Utilities for a Single Actor Unit per tick
 */

#ifndef GUARD_RULES_H
#define GUARD_RULES_H

#include "state.h"
#include "utilities.h"
#include "player_state_handler/player_state_handler.h"

#include <bits/stdc++.h>

namespace rules {

/**
 * Comparison function for the Sort function used in GetEnemyAllyNumRatio
 *
 * @param[in]  a     Comparison parameter 1
 * @param[in]  b     Comparison parameter 2
 *
 * @return     boolean var indicating whether a is lesser than b
 */
bool SortBySecondElement(const pair<state::act_id_t, float> &a, const pair<state::act_id_t, float> &b) {
	return (a.second < b.second);
}

/**
 * Function to find the ratio of Enemies to Ally from a given Unit's k neighbours
 *
 * @param[in]  state  The state
 * @param[in]  id     The identifier for the Unit
 * @param[in]  k      Number of neighbours to consider
 *
 * @return     The enemy ally number ratio.
 */
float GetEnemyAllyNumRatio(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id, int k) {
	state::UnitView parentUnit = state->GetUnitFromId(id, NULL);
	state::list_act_id_t allies = state->GetPlayerUnitIds();
	state::list_act_id_t enemies = state->GetEnemyUnitIds();
	vector<pair<state::act_id_t, float>> allypairs;
	vector<pair<state::act_id_t, float>> enemypairs;
	for (int i = 0; i < allies.size(); i++) {
		state::UnitView allyUnit = state->GetUnitFromId(allies[i], NULL);
		allypairs.push_back(make_pair(allies[i], allyUnit.GetPosition().distance(parentUnit.GetPosition())));
	}
	for (int i = 0; i < enemies.size(); i++) {
		state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], NULL);
		enemypairs.push_back(make_pair(enemies[i], enemyUnit.GetPosition().distance(parentUnit.GetPosition())));
	}
	std::sort(allypairs.begin(), allypairs.end(), SortBySecondElement);
	std::sort(enemypairs.begin(), enemypairs.end(), SortBySecondElement);
	float allyCount = 0;
	float enemyCount = 0;
	for (int i = 0, int ai = 0, int ei = 0; i < k; i++) {
		if (ai < allypairs.size() && ei < enemypairs.size()) {
			if (enemypairs[i].second < allypairs[i].second) {
				enemyCount++;
				ei++;
			}
			else {
				allyCount++;
				ai++;
			}
		}
		else if (ai < allypairs.size()) {
			allyCount++;
			ai++;
		}
		else if (ei < enemypairs.size()) {
			enemyCount++;
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
	state::UnitView parentUnit = state->GetUnitFromId(id, NULL);
	state::list_act_id_t enemies = state->GetEnemyUnitIds();
	state::act_id_t nearestEnemyId = -1;
	float nearestEnemyDistance = -1, dist;
	for (int i = 0; i < enemies.size(); i++) {
		state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], NULL);
		dist = enemyUnit[i].GetPosition().distance(parentUnit.GetPosition());
		if (nearestEnemyDistance == -1) {
			nearestEnemyDistance = dist;
			nearestEnemyId = enemyUnit[i].GetId();
		}
		else if (dist < nearestEnemyDistance) {
			nearestEnemyDistance = dist;
			nearestEnemyId = enemyUnit[i].GetId();
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
	state::UnitView parentUnit = state->GetUnitFromId(id, NULL);
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
	state::UnitView parentUnit = state->GetUnitFromId(id, NULL);
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
	state::UnitView parentUnit = state->GetUnitFromId(id, NULL);
	float baseDistance = state->GetBase().GetPosition().distance(parentUnit.GetPosition());
	if (nearestEnemyDistance <= parentUnit.GetAttackRange() && baseDistance == 0) return true;
	else return false;
}

/**
 * Class for guard rules for guard strategies and utilities
 */
class GuardRules {
	/**
	 * The given state of the game
	 */
	std::shared_ptr<state::PlayerStateHandler> state;
	/**
	 * The formation generally used for defense
	 */
	state::FormationMaker *formation;
public:
	GuardRules(std::shared_ptr<state::PlayerStateHandler> playerStateHandler, state::FormationMaker *formationMaker);
	/**
	 * The strategy for the defense per unit per tick
	 *
	 * @param[in]  unitId  The unit identifier
	 */
	void Strategy(state::act_id_t unitId);
	/**
	 * The utility function or the transition specifier for the Guard state
	 *
	 * @param[in]  unitId  The unit identifier
	 *
	 * @return     int var (1-GoToAtack, 2-GoToRetreat, 3-GoToExplore, 4-GoToGuard)
	 */
	int Utility(state::act_id_t unitId);
};

GuardRules::GuardRules(std::shared_ptr<state::PlayerStateHandler> playerStateHandler, state::FormationMaker *formationMaker) {
	state = playerStateHandler;
	formation = formationMaker;
}

void GuardRules::Strategy(state::act_id_t unitId) {
	if (state->GetUnitFromId(unitId).GetVelocity().magnitude() > 0 || state->GetUnitFromId(unitId).GetPathPlannerHelper().IsPathPlanning()) {
		return;
	}
	vector<int> terrain_weights(3);
	if (state->GetUnitFromId(unitId).GetActorType() == state::ActorType::MAGICIAN) {
		terrain_weights[state::MOUNTAIN] = 1;
		terrain_weights[state::PLAIN] = 2;
		terrain_weights[state::FOREST] = 4;
	}
	else if (state->GetUnitFromId(unitId).GetActorType() == state::ActorType::SWORDSMAN) {
		terrain_weights[state::MOUNTAIN] = 2;
		terrain_weights[state::PLAIN] = 2;
		terrain_weights[state::FOREST] = 1;
	}
	else if (state->GetUnitFromId(unitId).GetActorType() == state::ActorType::KING) {
		terrain_weights[state::MOUNTAIN] = 1;
		terrain_weights[state::PLAIN] = 1;
		terrain_weights[state::FOREST] = 4;
	}
	else if (state->GetUnitFromId(unitId).GetActorType() == state::ActorType::SCOUT) {
		terrain_weights[state::MOUNTAIN] = 1;
		terrain_weights[state::PLAIN] = 1;
		terrain_weights[state::FOREST] = 1;
	}
	if (state->GetFlag().GetVelocity().magnitude() > 0 || GetEnemyAllyNumRatio(state, state->GetFlag().GetId(), 20) > 1) {
		pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetFlag().GetId());
		if (state->GetUnitFromId(nearestEnemy.first).GetPosition().distance(state->GetUnitFromId(unitId).GetPosition()) <= state->GetUnitFromId(unitId).GetAttackRange()) {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, NULL);
		}
		else {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetFlag().GetPosition(), formation, terrain_weights, tempPath, NULL);
		}
	}
	else if (GetEnemyAllyNumRatio(state, state->GetKing().GetId(), 20) > 1) {
		pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetKing().GetId());
		if (state->GetUnitFromId(nearestEnemy.first).GetPosition().distance(state->GetUnitFromId(unitId).GetPosition()) <= state->GetUnitFromId(unitId).GetAttackRange()) {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, NULL);
		}
		else {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetKing().GetPosition(), formation, terrain_weights, tempPath, NULL);
		}
	}
	else if (GetEnemyAllyNumRatio(state, state->GetBase().GetId(), 20) > 1) {
		if (state->GetBase().GetPosition().distance(state->GetUnitFromId(unitId).GetPosition()) == 0) {
			pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetBase().GetId());
			list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, NULL);
		}
		else {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetBase().GetPosition(), formation, terrain_weights, tempPath, NULL);
		}
	}
	else {
		state::act_id_t weakestTowerId = -1;
		float weakestTowerHp = -1, hp;
		std::vector<state::TowerView> towers = state->GetTowers();
		for (int i = 0; i < towers.size(); i++) {
			hp = towers[i].GetHp();
			if (weakestTowerHp == -1) {
				weakestTowerHp = hp;
				weakestTowerId = towers[i].GetId();
			}
			else if (hp < weakestTowerHp) {
				weakestTowerHp = hp;
				weakestTowerId = towers[i].GetId();
			}
		}
		if (state->GetUnitFromId(weakestTowerId).GetPosition().distance(state->GetUnitFromId(unitId).GetPosition()) == 0) {
			pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, weakestTowerId);
			list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy.first, NULL);
		}
		else {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, state->GetUnitFromId(weakestTowerId).GetPosition(), formation, terrain_weights, tempPath, NULL);
		}
		
	} 
}

int GuardRules::Utility(state::act_id_t unitId) {
	float enemyAllyNumRatio = GetEnemyAllyNumRatio(state, unitId, 20);
	pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, unitId);
	bool isDefendingKing = IsDefendingKing(state, unitId, nearestEnemy.second);
	bool isDefendingTower = IsDefendingTower(state, unitId, nearestEnemy.second);
	bool isDefendingBase = IsDefendingBase(state, unitId, nearestEnemy.second);
	if (enemyAllyNumRatio == 0) {
		return 3;
	}
	else if (isDefendingKing || isDefendingTower || isDefendingBase) {
		return 4;
	}
	else if (enemyAllyHpRatio <= 1) {
		return 1;
	}
	else if (enemyAllyNumRatio > 1 && !isDefendingBase && !isDefendingTower && !isDefendingKing) {
		return 2;
	}
}

}

#endif
