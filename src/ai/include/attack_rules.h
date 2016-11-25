/**
 * @file attack_rules.h
 * Implementation file of the Attack Strategies and Utilities for a Single Actor Unit per tick
 */
#ifndef AI_ATTACK_RULES_H
#define AI_ATTACK_RULES_H

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
};

}
#endif // AI_ATTACK_RULES_H