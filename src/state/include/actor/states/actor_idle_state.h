/**
 * @file actor_idle_state.h
 * Idle state of Actor
 */

#ifndef STATE_ACTOR_STATES_ACTOR_IDLE_STATE_H
#define STATE_ACTOR_STATES_ACTOR_IDLE_STATE_H

#include <cstdint>
#include <memory>
#include "actor/actor.fwd.h"
#include "actor/states/actor_state.h"

namespace state {

/**
 * Idle state of Actor
 */
class ActorIdleState : public ActorState {
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
	 * Else if the Actor is path planning, returns
	 * ActorPathPlanningState.
	 * Else returns nullptr
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
	 * @param      actor  The Actor
	 */
	virtual void Exit(Actor * actor) override;
	/**
	 * Creates a new instance of the object with same properties of
	 * original
	 *
	 * @return     Copy of this object
	 */
	virtual std::unique_ptr<ActorState> Clone() override;
};

}

#endif
