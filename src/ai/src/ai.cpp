class Group {

private:
	GroupState* state;
public:
	virtual void update(Group& unit) 
	{
		state->update(*this);
	};
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
