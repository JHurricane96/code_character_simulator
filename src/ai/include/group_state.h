/**
 * @file group_state.h
 * The group states that define the state of being
 * for each individual unit.
*/
#ifndef AI_GROUP_STATE
#define AI_GROUP_STATE

#include <memory>
#include <map>
#include "player_ai_helper.h"
#include "player_state_handler/player_state_handler.h"
#include "attack_rules.h"
#include "guard_rules.h"

namespace ai {

/**
 * Base class for state from which other more definitive states
 * are derived that defines a unit's behaviour during an update.
 */
class GroupState
{
public:

	/**
	 * Constructor for group state
	 */
	GroupState() {};

	/**
	 * Updates the state of the unit every tick.
	 *
	 * @param[in]  unitId         The unit identifier
	 * @param[in]  state_handler  The state handler
	 * @param[in]  sortedEnemies  The sorted enemies
	 * @param      kingsGuard     The kings guard
	 * @param      flagsGuard     The flags guard
	 *
	 * @return     Returns the state the unit needs to be in for the next
	 * update
	 */
	virtual GroupState* update (
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state_handler,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	) {};

};

/**
 * Class for attack state.
 */
class Attack : public GroupState
{
	/**
	 * Utility value reflecting state change
	 * 1: Attack
	 * 2: Guard
	 */
	int groupUtilityHolder;

public:

	/**
	 * Attack rules member that defines strategy for the state
	 */
	AttackRules * attack_rules = new AttackRules();

	/**
	 * Updates the unit's state
	 *
	 * @param[in]  unitId         The unit identifier
	 * @param[in]  state_handler  The state handler
	 * @param[in]  sortedEnemies  The sorted enemies
	 * @param      kingsGuard     The kings guard
	 * @param      flagsGuard     The flags guard
	 *
	 * @return     Returns new state
	 */
	GroupState* update (
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state_handler,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	);

	/**
	 * Selects the new state for the unit
	 *
	 * @param[in]  unitId         The unit identifier
	 * @param[in]  state_handler  The state handler
	 *
	 * @return     Returns new state
	 */
	GroupState* SelectState (
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state_handler
	);
};

/**
 * Class for guard state
 */
class Guard : public GroupState
{
	/**
	 * Utility value reflecting state change
	 * 1: Attack
	 * 2: Guard
	 */
	int groupUtilityHolder;

public:

	/**
	 * Guard rules member that defines strategy for the state
	 */
	GuardRules * guard_rules = new GuardRules();

	/**
	 * Updates the unit's state
	 *
	 * @param[in]  unitId         The unit identifier
	 * @param[in]  state_handler  The state handler
	 * @param[in]  sortedEnemies  The sorted enemies
	 * @param      kingsGuard     The kings guard
	 * @param      flagsGuard     The flags guard
	 *
	 * @return     Returns new state
	 */
	GroupState* update(
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state_handler,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	);

	/**
	 * Selects the new state for the unit
	 *
	 * @param[in]  unitId         The unit identifier
	 * @param[in]  state_handler  The state handler
	 *
	 * @return     Returns new state
	 */
	GroupState* SelectState (
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state_handler
	);
};

/**
 * Class for group that can contain set of units
 * and associated update loop.
 */
class Group
{
private:
	/**
	 * A unique random id for the group
	 */
	int group_id;
	/**
	 * An id for the unit part of the group
	 */
	state::act_id_t unitId;
	/**
	 * The current state of behaviour the unit exhibits
	 */
	ai::GroupState* state;

public:

	/**
	 * Constructor for group.
	 *
	 * @param[in]  actid  The actid
	 */
	Group(state::act_id_t actid);

	/**
	 * Updates the group every tick.
	 *
	 * @param[in]  state_handler  The state handler
	 * @param[in]  sortedEnemies  The sorted enemies
	 * @param      kingsGuard     The kings guard
	 * @param      flagsGuard     The flags guard
	 */
	virtual void update (
		std::shared_ptr<state::PlayerStateHandler> state_handler,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	);
};

}

#endif // AI_GROUP_STATE
