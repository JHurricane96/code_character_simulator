/**
 * @file actor_pursuit_state.h
 * Pursuit state of Actor
 */

#ifndef STATE_ACTOR_STATES_ACTOR_PURSUIT_STATE_H
#define STATE_ACTOR_STATES_ACTOR_PURSUIT_STATE_H

#include <memory>
#include "actor/actor.fwd.h"
#include "actor/states/actor_state.h"

namespace state {

/**
 * Pursuit state of Actor
 * 
 * Actor is in this state when it's too far from its attack_target.
 * It pursues it by moving directly towards it
 */
class ActorPursuitState : public ActorState {
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
	 * Else if the Actor is path planning, returns
	 * ActorPathPlanningState.
	 * Else if Actor's attack_target is dead, returns ActorIdleState.
	 * Else if the Actor is close enough to the target, returns
	 * ActorAttackState.
	 * Else returns nullptr.
	 * 
	 * Sets the Actor's velocity to point towards the target, with its
	 * magnitude = Actor's max_speed
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
		float delta_time
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
