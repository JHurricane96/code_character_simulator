#include <vector>
#include <map>
#include <cstdlib>
#include <functional>
#include <set>
#include <algorithm>
#include <cmath>
#include <memory>
#include "ai.h"
#include "attack_rules.h"
#include "guard_rules.h"
#include "ai_utilities.h"

#define mod 1000000000
typedef int64_t act_id_t;
typedef std::vector<int64_t> list_act_id_t;

namespace ai {

class GroupState
{
public:
	GroupState() {}; // this constructor should set the default starting state
	virtual GroupState* update (
		state::act_id_t unitId, 
		std::shared_ptr<state::PlayerStateHandler> state_handler,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	) {};
	GroupState* DefaultActionUtility() {}; // utility to decide what to set as default state
};	

class Attack : public GroupState
{
public:
	AttackRules * attack_rules = new AttackRules();
	GroupState* update (
		state::act_id_t unitId, 
		std::shared_ptr<state::PlayerStateHandler> state_handler,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	);

	GroupState* SelectState(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state_handler);
};

class Explore : public GroupState
{
	GroupState* update(Group *group, std::shared_ptr<state::PlayerStateHandler> state_handler)
	{
		return SelectState();
	}

	GroupState* SelectState() {
	}
	void ExploreStrategy() {};
	float AttackUtility() {};
	float RetreatUtility() {};
	float ExploreUtility() {};
	float GuardUtility() {};
};

class Retreat : public GroupState
{
	GroupState* update(Group *group, std::shared_ptr<state::PlayerStateHandler> state_handler)
	{
		return SelectState();
	}

	GroupState* SelectState() {
	}
	void RetreatStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Guard : public GroupState
{
	int groupUtilityHolder;
public:
	GuardRules * guard_rules = new GuardRules();
	GroupState* update(
		state::act_id_t unitId, 
		std::shared_ptr<state::PlayerStateHandler> state_handler, 
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	);

	GroupState* SelectState(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state_handler);
};

GroupState* Attack::update (
	state::act_id_t unitId, 
	std::shared_ptr<state::PlayerStateHandler> state_handler,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	std::vector<state::act_id_t> &kingsGuard,
	std::vector<state::act_id_t> &flagsGuard
) {
	attack_rules -> Strategy(unitId, state_handler, sortedEnemies);
	return SelectState(unitId, state_handler);
}

GroupState* Attack::SelectState(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state_handler) {
	int new_state_no = attack_rules -> Utility(unitId, state_handler);
	if(new_state_no == 4)
			return new Guard;
	return NULL; 
}

GroupState* Guard::update(
	state::act_id_t unitId, 
	std::shared_ptr<state::PlayerStateHandler> state_handler, 
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	std::vector<state::act_id_t> &kingsGuard,
	std::vector<state::act_id_t> &flagsGuard
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

class Group
{
private:
	int group_id;
	state::act_id_t unitId;
	GroupState* state;
	// list_act_id_t group_actors;	// actors in this group
public:
	Group(state::act_id_t actid) : state(new Guard())
	{
		unitId = actid;
		group_id = rand() % mod;
	}

	virtual void update (
		std::shared_ptr<state::PlayerStateHandler> state_handler, 
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		std::vector<state::act_id_t> &kingsGuard,
		std::vector<state::act_id_t> &flagsGuard
	) {
		GroupState* NewState = state->update(unitId, state_handler, sortedEnemies, kingsGuard, flagsGuard);
		if(NewState != NULL)
		{
			delete state;
			state = NewState;
		}
	};
};

AI::AI() {
	init_groups = false;
}

void AI::SetSortedEnemies (
	std::shared_ptr<state::PlayerStateHandler> state
) {

	auto enemies = state->GetPlayerEnemyIds();
	for(auto enemy : enemies) {
		sortedEnemies.push_back (
				std::make_pair (
					enemy,
					state->GetEnemyUnitFromId(enemy, nullptr).GetHp()
				)
			);
	}
	std::sort(sortedEnemies.begin(), sortedEnemies.end(), SortedBySecondElement);
	return;
}

void AI::Update(std::shared_ptr<state::PlayerStateHandler> state) {
	if (!init_groups){
		for (auto actid : state -> GetPlayerUnitIds()) {
			groups.push_back(new Group(actid));
		}
		init_groups = true;
	}

	AI::SetSortedEnemies(state);

	auto to_respawn_ids = state->GetRespawnables();

	for (auto to_respawn_id : to_respawn_ids) {
		if (rand() % 2 == 0)
			state->RespawnUnit(to_respawn_id, state->GetBase().GetId(), NULL);
		else {
			auto towers = state->GetTowers();
			int chosen = rand() % towers.size();
			state->RespawnUnit(to_respawn_id, towers[chosen].GetId(), NULL);
		}
	}
	
	for (auto &group : groups)
		group -> update(state, sortedEnemies, kingsGuard, flagsGuard);
}

}
