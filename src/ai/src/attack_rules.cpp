/**
 * @file guard_rules.h
 * Implementation file of the Guard Strategies and Utilities for a Single Actor Unit per tick
 */

#ifndef ATTACK_RULES_H
#define ATTACK_RULES_H

#include "state.h"
#include "utilities.h"
#include "player_state_handler/player_state_handler.h"

#include <bits/stdc++.h>

#define INF 10000
namespace rules {

/**
 * Comparison function for the Sort function used in GetGetEnemyAllyHpRatio
 *
 * @param[in]  a     Comparison parameter 1
 * @param[in]  b     Comparison parameter 2
 *
 * @return     boolean var indicating whether a is lesser than b
 */
bool SortBySecondElement(const pair<state::act_id_t, float> &a, const pair<state::act_id_t, float> &b) {
	return (a.second < b.second);
}

float relu(float x) {
	if (x > 0) return x;
	return 0;
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
float GetEnemyAllyHpRatio(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id, int k) {
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
				enemyCount += state->getEnemyUnitFromId(enemypairs[i].first, NULL)->GetHp();
				ei++;
			}
			else {
				allyCount += state -> GetUnitFromId(allypairs[i].first, NULL) -> GetHp();
				ai++;
			}
		}
		else if (ai < allypairs.size()) {
			allyCount += state -> GetUnitFromId(allypairs[i].first, NULL) -> GetHp();
			ai++;
		}
		else if (ei < enemypairs.size()) {
			enemyCount += state->getEnemyUnitFromId(enemypairs[i].first, NULL)->GetHp();
			ei++;
		}
	}
	if (allyCount == 0) allyCount++;
	return enemyCount / allyCount;
}

/**
 * Fuction to find distance of a unit from a given unit
 *
 * @param[in]  unitId   	ID of given unit
 * @param[in]  unitview 	unit view of the unit to get distance from 
 *
 * @return     float distance
 */

float getDistanceFromUnit(state::act_id_t unitId, unitview) {
	if (unitview -> unit == nullptr) return INF;
	return unitview.GetPosition().distance(state -> GetUnitFromId(unitId).GetPosition());
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
 * Function to speculate whether a given Unit is attacking or not
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 *
 * @return     True if attacking an enemy unit/tower, False otherwise.
 */
bool IsAttackingEnemy(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id) {
	return GetUnitFromId(id) -> GetAttackTarget() != nullptr;
}

/**
 * Function to speculate whether a given Unit is attacking king or not
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 *
 * @return     True if attacking king, False otherwise.
 */
bool IsAttackingKing(std::shared_ptr<state::PlayerStateHandler> state, state::act_id_t id) {
	return GetUnitFromId(id) -> GetAttackTarget() -> GetId() == state -> GetEnemyKing(NULL) -> GetId();
}

/**
 * Class for attack rules for guard strategies and utilities
 */
class AttackRules {
	/**
	 * The given state of the game
	 */
	std::shared_ptr<state::PlayerStateHandler> state;
	/**
	 * The formation generally used for attack
	 */
	state::FormationMaker *formation;
public:
	AttackRules(std::shared_ptr<state::PlayerStateHandler> playerStateHandler, state::FormationMaker *formationMaker);
	/**
	 * The strategy for the attack per unit per tick
	 *
	 * @param[in]  unitId  The unit identifier
	 */
	void Strategy(state::act_id_t unitId);
	/**
	 * The utility function or the transition specifier for the Attack state
	 *
	 * @param[in]  unitId  The unit identifier
	 *
	 * @return     int var (1-GoToAtack, 2-GoToRetreat, 3-GoToExplore, 4-GoToGuard)
	 */
	int Utility(state::act_id_t unitId);
};

AttackRules::AttackRules(std::shared_ptr<state::PlayerStateHandler> playerStateHandler, state::FormationMaker *formationMaker) {
	state = playerStateHandler;
	formation = formationMaker;
}

bool inAttackRange(state::act_id_t unitId, EnemyUnitView enemy) {
	return (enemy.GetPosition().distance(state -> GetUnitFromId(unitId).GetPosition()) <= state -> GetUnitFromId(unitId).GetAttackRange());
}

void AttackRules::Strategy(state::act_id_t unitId) {
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

	/* Try to attack king if in LOS */
	if (state->GetEnemyKing(NULL)->unit != nullptr) {
		EnemyUnitView king = state->GetEnemyKing(NULL);
		if (inAttackRange(unitId, king)) {
			list_act_id_t attackers;
			attackers.push_back(king);
			state -> AttackUnit(attackers, king, NULL);
		}
		else {
			list_act_id_t attackers;
			attackers.push_back(unitId);
			std::vector<physics::Vector2D> tempPath;
			state->MoveUnits(attackers, king.GetPosition() / 2.0, formation, terrain_weights, tempPath, NULL);
		}
	} 

	/* Otherwise attack nearest enemy */
	else if (GetEnemyAllyHpRatio(state, unitId, 20) < 1) {
		pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetFlag().GetId());
		if (inAttackRange(unitId, state -> GetEnemyUnitFromId(nearestEnemy.first))) {
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

	/* Otherwise attack weakest tower in LOS */
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
		if (weakestTowerId == -1) {}
		else if (state->GetUnitFromId(weakestTowerId).GetPosition().distance(state->GetUnitFromId(unitId).GetPosition()) == 0) {
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

int AttackRules::Utility(state::act_id_t unitId) {
	float enemyAllyHpRatio = GetEnemyAllyHpRatio(state, unitId, 20);
	float distFromEnemyBase = getDistanceFromUnit(unitId, state -> GetEnemyBase());
	float distFromBase = getDistanceFromUnit(unitId, state -> GetBase());
	float distFromKing = getDistanceFromUnit(unitId, state -> GetKing());
	float distFromEnemyKing = getDistanceFromUnit(unitId, state -> GetEnemyKing());
	bool isAttackingKing = IsAttackingKing(state, unitId, nearestEnemy.second);
	bool isAttackingEnemy = IsAttackingEnemy(state, unitId, nearestEnemy.second);
	float eaHPratioKing = GetEnemyAllyHpRatio(state, state -> GetKing().GetId(), 20);
	float eaHPratioBase = GetEnemyAllyHpRatio(state, state -> GetBase().GetId(), 20);

	if (isAttackingKing) return 1;

	float attackUtility = (1.0 / enemyAllyHpRatio) * (1.0 / sqrt(distFromBase) + 1.0 / distFromEnemyBase + 1.0 / sqrt(distFromKing) + 1.0 / sqrt(distFromEnemyKing)); 
	float guard_utility = relu(eaHPratioBase - 0.8) + relu(eaHPratioKing - 1);
	float retreat_utility = 1. / (attackUtility + 0.00001);
	float explore_utility = 1. / distFromBase + 1. / (attackUtility + guard_utility + 0.00001);

	vector <float> utilities(4);
	utilities.push_back(attackUtility);
	utilities.push_back(guard_utility);
	utilities.push_back(retreat_utility);
	utilities.push_back(explore_utility);

	float maxutility = max_element(utilities.begin(), utilities.end());
	if (maxutility == attackUtility) return 1;
	if (maxutility == guard_utility) return 4;
	if (maxutility == retreat_utility) return 2;
	if (maxutility == explore_utility) return 3;
}

}

#endif
