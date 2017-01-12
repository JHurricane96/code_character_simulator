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