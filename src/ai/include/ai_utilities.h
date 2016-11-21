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

}

#endif // AI_AI_UTILITIES_H