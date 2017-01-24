#include <vector>
#include <map>
#include <cstdlib>
#include <functional>
#include <set>
#include <algorithm>
#include <cmath>

#define mod 1000000000
typedef int64_t act_id_t;
typedef std::vector<int64_t> list_act_id_t;

/**
  * The Main AI Class
  */
class AI 
{
public:
	list_act_id_t all_actors;
	map <int, Group*> groups;
	AI() 
	{
		// todo: initialise list of actor ids before passing
		Group *begin = Group(all_actors);
	}

	void update() 
	{
		// see how state object is passed to this function and pass it down
		for (auto &group : groups) (group.second) -> update(); 	
	}
}

class Group : AI
{
private:
	int group_id;
	GroupState* state;
	list_act_id_t group_actors;	// actors in this group
public:
	Group(list_act_id_t actors, GroupState& cur_state=GroupState()) 
	{
		group_actors = actors;
		group_id = rand() % mod;
		groups[group_id] = *this;
		state = cur_state;
	}

	virtual void update() 
	{
		GroupState* NewState = state->update(*this);
		if(NewState != null)
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
	
	void split(vector <int> hps)
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
	}
};

class GroupState
{
public:
	GroupState() {}; // this constructor should set the default starting state
	virtual GroupState* update(Group *group) {};
	GroupState* DefaultActionUtility() {}; // utility to decide what to set as default state
}	

class Attack : public GroupState
{
	GroupState* update(Group *group)
	{
		return SelectState();
	}

	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return NULL;
		else if(RetreatUtility == MaxUtility)
			return new Retreat();
		else if(ExploreUtility == MaxUtility)
			return new Explore();
		else if(GuardUtility == MaxUtility)
			return new Guard();
	}
	void AttackStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Explore : public GroupState
{
	GroupState* update(Group *group)
	{
		return SelectState();
	}

	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return new Attack();
		else if(RetreatUtility == MaxUtility)
			return new Retreat();
		else if(ExploreUtility == MaxUtility)
			return NULL;
		else if(GuardUtility == MaxUtility)
			return new Guard();
	}
	void ExploreStrategy() {};
	float AttackUtility() {};
	float RetreatUtility() {};
	float ExploreUtility() {};
	float GuardUtility() {};
};

class Retreat : public GroupState
{
	GroupState* update(Group *group)
	{
		return SelectState();
	}

	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return new Attack();
		else if(RetreatUtility == MaxUtility)
			return NULL;
		else if(ExploreUtility == MaxUtility)
			return new Explore();
		else if(GuardUtility == MaxUtility)
			return new Guard();
	}
	void RetreatStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Guard : public GroupState
{
	GroupState* update(Group *group)
	{
		return SelectState();
	}

	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return new Attack();
		else if(RetreatUtility == MaxUtility)
			return new Retreat();
		else if(ExploreUtility == MaxUtility)
			return new Explore();
		else if(GuardUtility == MaxUtility)
			return NULL;
	}
	void GuardStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};