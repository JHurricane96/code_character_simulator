#include <vector>
#include <map>
#include <cstdlib>
#include <functional>
#include <set>

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
	Group(list_act_id_t actors) 
	{
		group_actors = actors;
		group_id = rand() % mod;
		groups[group_id] = *this;	
	}

	virtual void update() 
	{
		state->update(*this);
	};

	void split(vector <int> hps)
	{
		sort(hps.begin(), hps.end(), std::greater<int>());
		set < pair <int, act_id_t> > group_hps;
		for (auto &id: group_actors) 
		{
			int hp = // get actor hp
			group_hps.insert(make_pair(hp, id));
		}

		for ()
	}

	virtual void AttackUtility() {};
	virtual void RetreatUtility() {};
	virtual void ExploreUtility() {};
	virtual void GuardUtility() {};
};

class GroupState
{
public:
	virtual void update() {};
}	

class Attack : public GroupState
{
public:
	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return null;
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
public:
	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return new Attack();
		else if(RetreatUtility == MaxUtility)
			return new Retreat();
		else if(ExploreUtility == MaxUtility)
			return null;
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
public:
	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return new Attack();
		else if(RetreatUtility == MaxUtility)
			return null;
		else if(ExploreUtility == MaxUtility)
			return null;
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
public:
	GroupState* SelectState() {
		float MaxUtility = MaxScore(AttackUtility(),RetreatUtility(),ExploreUtility(),GuardUtility());
		if(AttackUtility == MaxUtility)
			return new Attack();
		else if(RetreatUtility == MaxUtility)
			return new Retreat();
		else if(ExploreUtility == MaxUtility)
			return Explore();
		else if(GuardUtility == MaxUtility)
			return null;
	}
	void GuardStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};
