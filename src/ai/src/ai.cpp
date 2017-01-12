class Unit {

private:
	UnitState* state;
public:
	virtual void update(Unit& unit) 
	{
		state->update(*this);
	};
	virtual void AttackUtility() {};
	virtual void RetreatUtility() {};
	virtual void ExploreUtility() {};
	virtual void GuardUtility() {};
};

class UnitState
{
public:
	virtual void update() {};
}

class Attack : public UnitState
{
public:
	void AttackStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Explore : public UnitState
{
public:
	void ExploreStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Retreat : public UnitState
{
public:
	void RetreatStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};

class Guard : public UnitState
{
public:
	void GuardStrategy() {};
	void AttackUtility() {};
	void RetreatUtility() {};
	void ExploreUtility() {};
	void GuardUtility() {};
};
