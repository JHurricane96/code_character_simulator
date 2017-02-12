#include "player_state_handler/player_state_handler.h"
#include "group_state.h"

#define mod 1000000000

namespace ai {

Group::Group(state::act_id_t actid) : state(new Guard()) {
		unitId = actid;
		group_id = rand() % mod;
}

void Group::update (
	std::shared_ptr<state::PlayerStateHandler> state_handler,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	std::vector<state::act_id_t> &kingsGuard,
	std::vector<state::act_id_t> &flagsGuard,
	int aiLevel
) {
	ai::GroupState* NewState = state->update(unitId, state_handler, sortedEnemies, kingsGuard, flagsGuard, aiLevel);
	if(NewState != NULL)
	{
		delete state;
		state = NewState;
	}
}

GroupState* Attack::update (
	state::act_id_t unitId,
	std::shared_ptr<state::PlayerStateHandler> state_handler,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	std::vector<state::act_id_t> &kingsGuard,
	std::vector<state::act_id_t> &flagsGuard,
	int aiLevel
) {
	groupUtilityHolder = 0;
	attack_rules -> Strategy(unitId, state_handler, groupUtilityHolder, sortedEnemies, aiLevel);
	return SelectState(unitId, state_handler);
}

GroupState* Attack::SelectState(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state_handler) {
	int new_state_no = attack_rules -> Utility(groupUtilityHolder);
	if(new_state_no == 4)
			return new Guard;
	return NULL;
}

GroupState* Guard::update(
	state::act_id_t unitId, 
	std::shared_ptr<state::PlayerStateHandler> state_handler,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	std::vector<state::act_id_t> &kingsGuard,
	std::vector<state::act_id_t> &flagsGuard,
	int aiLevel
) {
	groupUtilityHolder = 0;
	guard_rules -> Strategy(unitId, state_handler, kingsGuard, flagsGuard, groupUtilityHolder, sortedEnemies);
	return SelectState(unitId, state_handler);
}

GroupState* Guard::SelectState(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state_handler) {
	int new_state_no = guard_rules -> Utility(groupUtilityHolder);
	if(new_state_no == 1)
		return new Attack;
	return NULL;
}

}
