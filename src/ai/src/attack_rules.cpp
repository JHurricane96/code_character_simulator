#include "state.h"
#include "utilities.h"
#include "player_state_handler/player_state_handler.h"
#include "attack_rules.h"

#include <bits/stdc++.h>

#define INF 10000
#define TINY 0.00001
#define SWORDHIT 100
#define MAGEHIT 150
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

		std::sort(allyPairs.begin(), allyPairs.end(), SortBySecondElement);
		std::sort(enemyPairs.begin(), enemyPairs.end(), SortBySecondElement);

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
 * Function to find nearest enemy of a particular type if specified from a given unit
 *
 * @param[in]  state  The state
 * @param[in]  id     The identifier for the Unit
 * @param[in]  type   The type of enemy unit being searched for
 *
 * @return     A pair object containing (NearestEnemyUnitId, NearestEnemyUnitDistance)
 */
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

/**
 * Function to find weakest enemy in a particular radius
 *
 * @param[in]  state       The state
 * @param[in]  id          The identifier
 * @param[in]  threshhold  The threshhold radius
 *
 * @return     A pair object containing (WeakestEnemyUnitId, WeakestEnemyUnitHP)
 */
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
		return (state -> GetUnitFromId(id, nullptr).GetAttackTarget(nullptr) != nullptr);
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

/**
 * Check if enemy unit is within range to attack
 *
 * @param[in]  state   The state
 * @param[in]  unitId  The unit identifier
 * @param[in]  enemy   The enemy
 *
 * @return     True if in range, False otherwise.
 */
bool InAttackRange (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId,
	state::EnemyUnitView enemy
)	{
	auto unit = state -> GetUnitFromId(unitId, nullptr);
	return (enemy.GetPosition().distance(unit.GetPosition()) <= unit.GetAttackRange());
}

/**
 * Strategy to effectively attack enemies whilst in attack state
 *
 * @param[in]  unitId  The unit identifier
 * @param[in]  state   The state
 */
void AttackRules::Strategy (
	state::act_id_t unitId,
	std::shared_ptr<state::PlayerStateHandler> state
)	{
		if (state->GetUnitFromId(unitId, nullptr).GetVelocity().magnitude() > 0
			|| state->GetUnitFromId(unitId, nullptr).GetPathPlannerHelper().IsPathPlanning()
			|| !state->GetUnitFromId(unitId, nullptr).CanAttack()
			|| !state->GetUnitFromId(unitId, nullptr).CanPathPlan()) {
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

		std::unique_ptr<int> success(new int());
		auto enemy_king = state -> GetEnemyKing(success.get());
		float AttackValidity = GetEnemyAllyHpRatio(state, unitId, 20);

		/* Try to attack king if in LOS */

		if (*success == 1) {
			if (InAttackRange(state, unitId, enemy_king)) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state -> AttackUnit(attackers,enemy_king.GetId(), nullptr);
			}
			else {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				std::vector<physics::Vector2D> tempPath;
				state->MoveUnits (
					attackers,
					enemy_king.GetPosition(),
					formation,
					terrain_weights,
					tempPath,
					nullptr);
			}
		}

		/* Check if wise to attack */

		else if (AttackValidity < 1 && AttackValidity != -1) {
			auto unit = state -> GetUnitFromId (unitId, nullptr);
			auto weakestEnemy = WeakestEnemy(state, unitId, unit.GetAttackRange());

			/* Try to attack weakest enemy in LOS */

			if(weakestEnemy.second < unit.GetHp() && (weakestEnemy.first != -1)) {
				if( InAttackRange(state, unitId, state -> GetEnemyUnitFromId(weakestEnemy.first, nullptr)) ) {
					state::list_act_id_t attackers;
					attackers.push_back(unitId);
					state -> AttackUnit(attackers, weakestEnemy.first, nullptr);
				}
				else {
						auto target = state->GetEnemyUnitFromId (weakestEnemy.first, nullptr);
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						std::vector<physics::Vector2D> tempPath;
						state->MoveUnits (
							attackers,
							target.GetPosition(),
							formation,
							terrain_weights,
							tempPath,
							nullptr);
				}
			}

			/* Otherwise pursue nearest enemy */

			else {

				/* Look for enemy's nearest scout */
				state::ActorType typeScout = state::ActorType::SCOUT;
				state::ActorType typeTower = state::ActorType::TOWER;
				state::ActorType typeBase = state::ActorType::BASE;
				state::ActorType typeSwordsman = state::ActorType::SWORDSMAN;
				state::ActorType typeMagician = state::ActorType::MAGICIAN;

				auto nearestEnemyScout = NearestEnemy(state, unitId, &typeScout);
				auto nearestEnemyTower = NearestEnemy(state, unitId, &typeTower);
				auto nearestEnemyBase = NearestEnemy(state, unitId, &typeBase);

				if( nearestEnemyScout.first != -1
					&& InAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemyScout.first, nullptr)) ) {
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						state->AttackUnit(attackers, nearestEnemyScout.first, nullptr);
					}

				/* Look for enemy's nearest tower */

				else if( nearestEnemyTower.first != -1) {
					if (InAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemyTower.first, nullptr)) ) {
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						state->AttackUnit(attackers, nearestEnemyTower.first, nullptr);
					}
					else {
						auto target = state->GetEnemyUnitFromId (nearestEnemyTower.first, nullptr);
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						std::vector<physics::Vector2D> tempPath;
						state->MoveUnits(attackers, target.GetPosition(), formation, terrain_weights, tempPath, nullptr);
					}
				}

				/* Look for enemy's base */

				else if( nearestEnemyBase.first != -1) {
					if (InAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemyBase.first, nullptr)) ) {
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						state->AttackUnit(attackers, nearestEnemyBase.first, nullptr);
					}
					else {
						auto target = state->GetEnemyUnitFromId (nearestEnemyBase.first, nullptr);
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						std::vector<physics::Vector2D> tempPath;
						state->MoveUnits(attackers, target.GetPosition(), formation, terrain_weights, tempPath, nullptr);
					}
				}

				/* Look for nearest enemy swordsman/mage */

				else {
					auto nearestEnemySwordsman = NearestEnemy(state, unitId, &typeSwordsman);
					auto nearestEnemyMagician = NearestEnemy(state, unitId, &typeMagician);
					std::pair<state::act_id_t, float> target(-1, FLT_MAX);

					if(nearestEnemySwordsman.first == -1 && nearestEnemyMagician.first != -1) {
						target = nearestEnemyMagician;
					}
					else if(nearestEnemySwordsman.first != -1 && nearestEnemyMagician.first == -1) {
						target = nearestEnemySwordsman;
					}
					else if(nearestEnemySwordsman.first != -1 && nearestEnemyMagician.first != -1) {
						auto swordsmanUnit = state -> GetEnemyUnitFromId(nearestEnemySwordsman.first, nullptr);
						auto magicianUnit = state -> GetEnemyUnitFromId(nearestEnemyMagician.first, nullptr);

						target = ( (MAGEHIT/(magicianUnit.GetHp() + TINY)) > (SWORDHIT/(swordsmanUnit.GetHp() + TINY)) ) 
						? nearestEnemyMagician : nearestEnemySwordsman;
					}

					if(target.first != -1) {
						if (InAttackRange(state, unitId, state -> GetEnemyUnitFromId(target.first, nullptr)) ) {
							state::list_act_id_t attackers;
							attackers.push_back(unitId);
							state->AttackUnit(attackers, target.first, nullptr);
						}
						else {
							auto targetUnit = state->GetEnemyUnitFromId (target.first, nullptr);
							state::list_act_id_t attackers;
							attackers.push_back(unitId);
							std::vector<physics::Vector2D> tempPath;
							state->MoveUnits(attackers, targetUnit.GetPosition(), formation, terrain_weights, tempPath, nullptr);
						}
					}
				}
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
	float retreat_utility = 1. / (attackUtility + TINY);
	float explore_utility = 1. / distFromBase + 1. / (attackUtility + guard_utility + TINY);

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
