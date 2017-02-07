/**
 * @file guard_rules.h
 * Implementation file for defence strategies
 */
#ifndef GUARD_RULES_H
#define GUARD_RULES_H

#include <memory>
#include "player_state_handler/player_state_handler.h"

namespace ai {

/**
 * Class for guard rules, defence strategies and utilities
 */
class GuardRules {
	/**
	 * The formation generally used for guarding
	 */
	state::FormationMaker *formation;

public:

	GuardRules();

	/**
	 * The strategy for guarding per unit per tick
	 *
	 * @param[in]  unitId  The unit identifier
	 */
	void Strategy (
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state
	);


	/**
	 * The utility function or the transition specifier for the guard state
	 *
	 * @param[in]  unitId  The unit identifier
	 *
	 * @return     int var (1-GoToAtack, 2-GoToRetreat, 3-GoToExplore, 4-GoToGuard)
	 */
	int Utility(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state);
};

}

#endif
