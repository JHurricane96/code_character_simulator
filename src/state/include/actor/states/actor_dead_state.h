/**
 * @file actor_dead_state.h
 * Dead state of Actor
 */

#ifndef STATE_ACTOR_STATES_ACTOR_DEAD_STATE_H
#define STATE_ACTOR_STATES_ACTOR_DEAD_STATE_H

#include <cstdint>
#include <memory>
#include "actor/actor.fwd.h"
#include "actor/states/actor_state.h"

namespace state {

/**
 * Dead state of Actor
 * 
 * Actor is in this state when it's dead
 */
class ActorDeadState : public ActorState {
public:
	/**
	 * Called right after the Actor switches to this state
	 * 
	 * Calls Actor's Die method
	 *
	 * @param      actor  The Actor
	 */
	virtual void Enter(Actor * actor) override;
	/**
	 * Called every tick by the Actor
	 *
	 * If the Actor's time_to_respawn is 0 and its respawn_location has
	 * been set, i.e., it isn't (-1, -1), returns ActorIdleState.
	 * Else returns nullptr.
	 *
	 * Decreases time_to_respawn each tick
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
	 * Calls Actor's Respawn method
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
