/**
 * @file ai_utilities.h
 * Utility functions for AI
*/

#ifndef AI_AI_UTILITIES_H
#define AI_AI_UTILITIES_H

#include "player_state_handler/player_state_handler.h"
#include <bits/stdc++.h>

namespace ai {

/**
* Comparison function for the sort function used in GetEnemyAllyHpRatio
*
* @param[in]  a     Comparison parameter 1
* @param[in]  b     Comparison parameter 2
*
* @return     Boolean var indicating whether a is lesser than b
*/
bool SortedBySecondElement (
	const std::pair<state::act_id_t,
	float> &a,
	const std::pair<state::act_id_t,
	float> &b
);

state::act_id_t NearestEnemyByType (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	std::vector<state::ActorType> types
);

/**
 * Returns positive if positive, else 0
 *
 * @param[in]  x	Value to be checked
 *
 * @return     The result
 */
float relu (float x);

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
);

float GetEnemyAllyHpRatioByThreshold (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t id,
	float threshold
);

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
);

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
);

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
);

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
);

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
);

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
);

class DefaultFormation : public state::FormationMaker {
	std::vector<physics::Vector2D> ReturnFormation(int64_t formation_size) override {
		return std::vector<physics::Vector2D>(formation_size, physics::Vector2D(0, 0));
	}
};

bool IsEnemyTowerDominating(std::shared_ptr<state::PlayerStateHandler> state);

state::act_id_t GetOptimalTarget (
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unitId,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	float threshold = -1
);

}

#endif // AI_AI_UTILITIES_H
