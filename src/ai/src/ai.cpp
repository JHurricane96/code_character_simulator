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
		//return new Attack;
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

	/*** Split, Merge and their utilities below this **/

	/**
	  * There can be at most one call to either split or merge function per time tick
	  * If multiple calls have to be made make sure these multiple calls operate on 
	    disjoint sets of actors. Not doing so would lead to runtime errors.
	  * The split function splits a group, given the HP distribution of the new groups.
	  * The merge function has to be passed a GroupState* indicating the state from which
	  	the new Group can resume their actions. Not doing so, would reset all of them to a
	  	default state.
	  */
	
/*	void split(vector <int> hps)
	{
		sort(hps.begin(), hps.end(), std::greater<int>());
		set < pair <int, act_id_t> > group_hps;
		vector < list_act_id_t > actor_splits(hps.size());
		for (auto &id: group_actors) 
		{
			int hp = // get current actor hp
			group_hps.insert(make_pair(hp, id));
		}
		// clip extra HP from requested distribution
		hps = clipExtra(hps, accumulate(group_hps.begin(), group_hps.end(), 0));
		int cur = 0; // index of HP in hps array being allocated
		int cur_split_index = 0;
		for (int i = 0; i < hps.size(); ++i)
		{
			int req_hp = hps[i];
			bool empty = true;
			set < pair <int, act_id_t> > :: iterator allocated = getCeil(group_hps, req_hp);
			while (alloc_hp != 0)
			{
				actor_splits[i].append(allocated -> second);
				empty = false;
				group_hps.erase(allocated);
				req_hp -= (allocated -> first);
				allocated = getCeil(group_hps, req_hp);
			}
			if (empty)
			{
				actor_splits[i].append(group_hps.begin() -> second);
				group_hps.erase(group_hps.begin())
			}
		}
		for (auto actors : actor_splits) Group(actors);
		groups.erase(groups.find(*this -> group_id));
	}
	void merge(vector <Group*> grps, GroupState* new_state=GroupState())
	{
		list_act_id_t new_actors;
		for (auto &group : grps)
		{
			for (auto &actor : group -> group_actors) 
				new_actors.append(actor);
		}
		Group(new_actors, new_state);
		for (auto &old_grp : grps) groups.erase(groups.find(old_grp -> group_id));
	}
	vector <int> clipExtra(vector <int> hps, int extra)
	{
		int residueOfEach = extra / hps.size();
		for (auto &hp: hps) hp -= residueOfEach;   // assuming this remains +ve
		return hps;
	}
	int getCeil(set < pair <int, act_id_t> > hps, x) 
	{
		set < pair <int, act_id_t> > :: iterator it = hps.lower_bound(x);
		if ((*it) -> first > x)
		{
			if (it == hps.begin()) return 0;
			return (--it) -> first;
		}
		return it -> first;
	}*/
};

/**
* Comparison function for the sort function used in GetEnemyAllyHpRatio
*
* @param[in]  a     Comparison parameter 1
* @param[in]  b     Comparison parameter 2
*
* @return     boolean var indicating whether a is lesser than b
*/


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
