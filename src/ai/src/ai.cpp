#include <vector>

typedef int64_t act_id_t;
typedef std::vector<int64_t> list_act_id_t;

/**
  * The Main AI Class
  */
class AI 
{
public:
	list_act_id_t all_actors;
	vector <Group*> groups;
	AI() 
	{
		// todo: initialise list of actor ids before passing
		Group *begin = Group(all_actors);
	}

	void update() 
	{
		for (auto &group : groups) group -> update();
	}
}

class Group : AI
{
private:
	GroupState* state;
	list_act_id_t group_actors;	// actors in this group
public:
	Group(list_act_id_t actors) 
	{
		this->group_actors = actors;
	}

	virtual void update() 
	{
		state->update(*this);
	};

	void split()
	{
		
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
	void AttackStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Explore : public GroupState
{
public:
	void ExploreStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Retreat : public GroupState
{
public:
	void RetreatStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Guard : public GroupState
{
public:
	void GuardStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};
