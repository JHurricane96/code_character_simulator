/**
 * @file actor_attack_state.h
 * Attack state of Actor
 */

#ifndef STATE_ACTOR_STATES_ACTOR_ATTACK_STATE_H
#define STATE_ACTOR_STATES_ACTOR_ATTACK_STATE_H

#include <cstdint>
#include <memory>
#include "actor/actor.fwd.h"
#include "actor/states/actor_state.h"

namespace state {

/**
 * Attack state of Actor
 * 
 * Actor is in this state when it's attacking and within range of the
 * target
 */
class ActorAttackState : public ActorState {
private:
	/**
	 * The time left to execute the next attack
	 * 
	 * Initially, it is the Actor's attack_speed.
	 * When it goes below zero, an attack is executed,
	 * upon which it is reset to attack_speed
	 */
	int64_t time_to_attack;
public:
	/**
	 * Called right after the Actor switches to this state
	 * 
	 * Sets time_to_attack to Actor's attack_speed
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
	 * Else if the Actor is too far from the target, returns
	 * ActorPursuitState.
	 * Else returns nullptr.
	 * 
	 * Damages Actor's attack_target when time_to_attack falls below
	 * zero, and resets time_to_attack
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
};

}

#endif
