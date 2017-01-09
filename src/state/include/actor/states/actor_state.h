/**
 * @file actor_state.h
 * Interface for any state of Actor
 */

#ifndef STATE_ACTOR_STATES_ACTOR_STATE_H
#define STATE_ACTOR_STATES_ACTOR_STATE_H

#include <memory>
#include "actor/actor.fwd.h"

namespace state {

/**
 * The base ActorState class
 * 
 * All states of Actor are derived from this class in accordance with
 * the state pattern.
 * An object of this class is meant to be a member of every Actor
 */
class ActorState {
public:
	/**
	 * Called right after the Actor switches to this state
	 *
	 * @param      actor  The Actor
	 */
	virtual void Enter(Actor * actor) = 0;
	/**
	 * Called every tick by the Actor
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
	) = 0;
	/**
	 * Called right before the Actor switches to another state
	 *
	 * @param      actor  The Actor
	 */
	virtual void Exit(Actor * actor) = 0;
	/**
	 * Creates a new instance of the object with same properties of
	 * original
	 *
	 * @return     Copy of this object
	 */
	virtual std::unique_ptr<ActorState> Clone() = 0;
};

}

#endif
