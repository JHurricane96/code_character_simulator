/**
 * @file attack_rules.h
 * Implementation file of the Attack Strategies and Utilities for a Single Actor Unit per tick
 */
#ifndef ATTACK_RULES_H
#define ATTACK_RULES_H

#include <memory>
#include "player_state_handler/player_state_handler.h"

namespace ai {

/**
 * Class for attack rules for guard strategies and utilities
 */
class AttackRules {

	/**
	 * The formation generally used for attack
	 */
	state::FormationMaker *formation;
public:

	AttackRules();

	/**
	 * The strategy for the attack per unit per tick
	 *
	 * @param[in]  unitId  The unit identifier
	 */
	void Strategy(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state);

	/**
	 * The utility function or the transition specifier for the Attack state
	 *
	 * @param[in]  unitId  The unit identifier
	 *
	 * @return     int var (1-GoToAtack, 2-GoToRetreat, 3-GoToExplore, 4-GoToGuard)
	 */
	int Utility(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state);
};

}

#endif
