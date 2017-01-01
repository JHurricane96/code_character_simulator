/**
 * @file actor_path_planning_state.h
 * Path planning state of Actor
 */

#ifndef STATE_ACTOR_STATES_ACTOR_PATH_PLANNING_STATE_H
#define STATE_ACTOR_STATES_ACTOR_PATH_PLANNING_STATE_H

#include <cstdint>
#include <memory>
#include "actor/actor.fwd.h"
#include "actor/states/actor_state.h"

namespace state {

/**
 * Path planning state of Actor
 * 
 * Actor is in this state when it's path planning
 */
class ActorPathPlanningState : public ActorState {
public:
	/**
	 * Called right after the Actor switches to this state
	 *
	 * @param      actor  The Actor
	 */
	virtual void Enter(Actor * actor) override;
	/**
	 * Called every tick by the Actor
	 * 
	 * If the Actor is dead, returns ActorDeadState.
	 * Else if the Actor has an attack_target, returns
	 * AttackPursuitState.
	 * Else if the Actor is done path planning, returns ActorIdleState.
	 * Else return nullptr.
	 *
	 * @param      actor       The Actor
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 *
	 * @return     If it's time to transit to another state, a pointer
	 *             to the new state is returned, else nullptr is
	 *             returned
	 */
	virtual std::unique_ptr<ActorState> Update(
		Actor * actor,
		int64_t delta_time
	) override;
	/**
	 * Called right before the Actor switches to another state
	 * 
	 * Calls Actor's PathPlannerHelper's FinishPath
	 *
	 * @param      actor  The Actor
	 */
	virtual void Exit(Actor * actor) override;
};

}

#endif
