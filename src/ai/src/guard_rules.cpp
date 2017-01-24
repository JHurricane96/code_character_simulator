/**
 * @file guard_rules.h
 * Implementation file of the Guard Strategies and Utilities
 * 
 * Brief descriptions-
 * Magician:
 * Swordsman: 
 * Tower: 
 * King:
 */

#ifndef GUARD_RULES_H
#define GUARD_RULES_H

namespace rules {

class GuardRules {
public:
	void Strategy(std::shared_ptr<state::PlayerStateHandler> state);
	float AttackUtility(std::shared_ptr<state::PlayerStateHandler> state);
	float RetreatUtility(std::shared_ptr<state::PlayerStateHandler> state);
	float ExploreUtility(std::shared_ptr<state::PlayerStateHandler> state);
	float GuardUtility(std::shared_ptr<state::PlayerStateHandler> state);
};

void GuardRules::Strategy(std::shared_ptr<state::PlayerStateHandler> state) {
//Ally Flag lifted strategy
//Ally player weak strategy
//Enemy player strong strategy
//Ally base defense or weak security strategy
//Ally tower defending strategy
//Ally king defending strategy
}

float GuardRules::AttackUtility(std::shared_ptr<state::PlayerStateHandler> state) {
//Go to attack if stronger than enemy
//Go to attack if flag capture or tower capture opportunity
}

float GuardRules::RetreatUtility(std::shared_ptr<state::PlayerStateHandler> state) {
//Go to retreat if weaker than enemy
}

float GuardRules::ExploreUtility(std::shared_ptr<state::PlayerStateHandler> state) {
//Go to explore if nothing else
}

float GuardRules::GuardUtility(std::shared_ptr<state::PlayerStateHandler> state) {
//Go to guard if weak ally unit
//Go to guard if weak base security or defense
//Go to guard if ally flag is lifted
//Go to guard if ally tower is attacked
//Go to guard if king is carrying the flag
}

}

#endif