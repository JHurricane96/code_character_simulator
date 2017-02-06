#include "state.h"
#include "utilities.h"
#include "player_state_handler/player_state_handler.h"
#include "attack_rules.h"

#include <bits/stdc++.h>

#define INF 10000
namespace ai {

class DefaultFormation : public state::FormationMaker {
	std::vector<physics::Vector2D> ReturnFormation(int64_t formation_size) override {
		return std::vector<physics::Vector2D>(formation_size, physics::Vector2D(0, 0));
	}
};

/**
 * Comparison function for the sort function used in GetEnemyAllyHpRatio
 *
 * @param[in]  a     Comparison parameter 1
 * @param[in]  b     Comparison parameter 2
 *
 * @return     boolean var indicating whether a is lesser than b
 */
bool SortBySecondElement (
	const std::pair<state::act_id_t,
	float> &a, const std::pair<state::act_id_t,
	float> &b
) 	{
		return (a.second < b.second);
}

float relu (float x) {
	if (x > 0)
		return x;
	return 0;
}

/**
 * Function to find the ratio of hp of enemies to allies in a given unit's k neighbours
 *
 * @param[in]  state  The state
 * @param[in]  id     The identifier for the Unit
 * @param[in]  k      Number of neighbours to consider
 *
 * @return     The enemy ally HP ratio.
 */
float GetEnemyAllyHpRatio (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	int k
)	{
		state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);

		std::vector<MagicianView> allyMagicians = state->GetMagicians();
		std::vector<SwordsmanView> allySwordsmen = state->GetSwordsmen();
		std::vector<TowerView> allyTowers = state->GetTowers();

		std::vector<EnemyMagicianView> enemyMagicians = state->GetEnemyMagicians();
		std::vector<EnemySwordsmanView> enemySwordsmen = state->GetEnemySwordsmen();
		std::vector<EnemyTowerView> enemyTowers = state->GetEnemyTowers();

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
		for (auto swordsmen : allySwordsmen) {
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
		for (int i = 0; i < enemies.size(); i++) {
			state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
			enemypairs.push_back (
				std::make_pair (
					enemyUnit.GetHp(),
					enemyUnit.GetPosition().distance(parentUnit.GetPosition())
				)
			);
		}

		std::sort(allyPairs.begin(), allyPairs.end(), SortBySecondElement);
		std::sort(enemyPairs.begin(), enemyPairs.end(), SortBySecondElement);

		float allyHp = 0, enemyHp = 0;
		int allyCount = 0, enemyCount = 0;

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
			else if (ai < allyPairs.size()) {
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

/**
 * Function to find distance of a unit from a given unit
 *
 * @param[in]  unitId   	ID of given unit
 * @param[in]  unitview 	unit view of the unit to get distance from 
 *
 * @return     float distance
 */

float GetDistanceFromUnit(
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId, physics::Vector2D position
)	{
		return position.distance(state -> GetUnitFromId(unitId, nullptr).GetPosition());
}

/**
 * Function to find nearest enemy from a given unit
 *
 * @param[in]  state  The state
 * @param[in]  id     The identifier for the Unit
 *
 * @return     A pair object containing (NearestEnemyUnitId, NearestEnemyUnitDistance)
 */
std::pair<state::act_id_t, float> NearestEnemy (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id
)	{

		state::UnitView parentUnit = state->GetUnitFromId(id, nullptr);
		state::list_act_id_t enemies = state->GetPlayerEnemyIds();

		state::act_id_t nearestEnemyId = -1;
		float nearestEnemyDistance = FLT_MAX, dist;

		for (int i = 0; i < enemies.size(); i++) {

			state::EnemyUnitView enemyUnit = state->GetEnemyUnitFromId(enemies[i], nullptr);
			dist = enemyUnit.GetPosition().distance(parentUnit.GetPosition());

			if (dist < nearestEnemyDistance) {
				nearestEnemyDistance = dist;
				nearestEnemyId = enemyUnit.GetId();
			}
		}
		return std::make_pair(nearestEnemyId, nearestEnemyDistance);
}

/**
 * Function to speculate whether a given unit is attacking or not
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 *
 * @return     True if attacking an enemy unit/tower, false otherwise.
 */
bool IsAttackingEnemy (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id
)	{
		return (state -> GetUnitFromId(id, nullptr).GetAttackTarget(nullptr) != nullptrptr);
}

/**
 * Function to speculate whether a given unit is attacking the king or not
 *
 * @param[in]  state                 The state
 * @param[in]  id                    The identifier for the Unit
 *
 * @return     True if attacking king, False otherwise.
 */
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

AttackRules::AttackRules() {
	formation = new DefaultFormation();
}

bool inAttackRange (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId,
	state::EnemyUnitView enemy
)	{
	auto unit = state -> GetUnitFromId(unitId, nullptr);
	return (enemy.GetPosition().distance(unit.GetPosition()) <= unit.GetAttackRange());
}

void AttackRules::Strategy (
	state::act_id_t unitId,
	std::shared_ptr<state::PlayerStateHandler> state
)	{
		if (state->GetUnitFromId(unitId, nullptr).GetVelocity().magnitude() > 0 || state->GetUnitFromId(unitId, nullptr).GetPathPlannerHelper().IsPathPlanning()) {
			return;
		}

		std::vector<int64_t> terrain_weights(3);
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

		/* Try to attack king if in LOS */
		std::unique_ptr<int> success(new int());
		auto enemy_king = state -> GetEnemyKing(success.get());

		if (*success == 1) {
			if (inAttackRange(state, unitId, enemy_king)) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state -> AttackUnit(attackers,enemy_king.GetId(), nullptr);
			}
			else {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				std::vector<physics::Vector2D> tempPath;
				state->MoveUnits(attackers, enemy_king.GetPosition(), formation, terrain_weights, tempPath, nullptr);
			}
		}

		/* Otherwise attack nearest enemy */
		else if (GetEnemyAllyHpRatio(state, unitId, 20) < 1) {
			std::pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, state->GetFlag().GetId());
			if (inAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemy.first, nullptr))) {
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
			else if (state->GetUnitFromId(weakestTowerId, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) == 0) {
				std::pair<state::act_id_t, float> nearestEnemy = NearestEnemy(state, weakestTowerId);
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

int AttackRules::Utility(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state) {
	float enemyAllyHpRatio = GetEnemyAllyHpRatio(state, unitId, 20);
	float distFromEnemyBase = GetDistanceFromUnit(state, unitId, state -> GetEnemyBase().GetPosition());
	float distFromBase = GetDistanceFromUnit(state, unitId, state -> GetBase().GetPosition());
	float distFromKing = GetDistanceFromUnit(state, unitId, state -> GetKing().GetPosition());
	std::unique_ptr<int> success(new int());
	auto enemy_king = state -> GetEnemyKing(success.get());
	float distFromEnemyKing = INF;
	if (*success == 1) {
		distFromEnemyKing = GetDistanceFromUnit(state, unitId, enemy_king.GetPosition());
	}
	bool isAttackingKing = IsAttackingKing(state, unitId);
	bool isAttackingEnemy = IsAttackingEnemy(state, unitId);
	float eaHPratioKing = GetEnemyAllyHpRatio(state, state -> GetKing().GetId(), 20);
	float eaHPratioBase = GetEnemyAllyHpRatio(state, state -> GetBase().GetId(), 20);

	if (isAttackingKing) return 1;

	float attackUtility = (1.0 / enemyAllyHpRatio) * (1.0 / sqrt(distFromBase) + 1.0 / distFromEnemyBase + 1.0 / sqrt(distFromKing) + 1.0 / sqrt(distFromEnemyKing)); 
	float guard_utility = relu(eaHPratioBase - 0.8) + relu(eaHPratioKing - 1);
	float retreat_utility = 1. / (attackUtility + 0.00001);
	float explore_utility = 1. / distFromBase + 1. / (attackUtility + guard_utility + 0.00001);

	std::vector <float> utilities(4);
	utilities.push_back(attackUtility);
	utilities.push_back(guard_utility);
	utilities.push_back(retreat_utility);
	utilities.push_back(explore_utility);

	float maxutility = *std::max_element(utilities.begin(), utilities.end());
	if (maxutility == attackUtility) return 1;
	if (maxutility == guard_utility) return 4;
	if (maxutility == retreat_utility) return 2;
	if (maxutility == explore_utility) return 3;
}

}
