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

}
