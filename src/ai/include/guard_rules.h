/**
 * @file guard_rules.h
 * Headers file for defence strategies
 */

#ifndef GUARD_RULES_H
#define GUARD_RULES_H

#include <memory>
#include <vector>
#include <bits/stdc++.h>

#include "state.h"
#include "ai_utilities.h"
#include "utilities.h"
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
	/**
	 * Constructor for the GuardRules class
	 *
	 * @param      formation       The formation
	 */
	GuardRules(state::FormationMaker *formationParam = new DefaultFormation());
	/**
	 * The strategy for guarding per unit per tick
	 *
	 * @param[in]  unitId            The unit identifier
	 * @param[in]  state             The state
	 * @param      groupStateHolder  The group state holder
	 */
	void Strategy(
		state::act_id_t unitId, 
		std::shared_ptr<state::PlayerStateHandler> state, 
		std::vector<state::act_id_t> &kingsGuard, 
		std::vector<state::act_id_t> &flagsGuard, 
		int &groupUtilityHolder
	);
	int Utility(int &groupStateHolder);
};

}

#endif
