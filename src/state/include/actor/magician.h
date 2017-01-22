/**
 * @file magician.h
 * Defines the Magician class
 */

#ifndef STATE_ACTOR_MAGICIAN_H
#define STATE_ACTOR_MAGICIAN_H

#include <memory>
#include "actor/actor.h"
#include "actor/fire_ball.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {
/**
 * Class for Magicians
 */
class STATE_EXPORT Magician: public Actor {
private:
	/**
	 * Speed of a fire_ball fired by the magician
	 */
	int64_t fire_ball_speed;
	/**
	 * Time to live for the fire_balls fired
	 */
	int64_t fire_ball_ttl;
	/**
	 * Size of the fire_balls fired
	 */
	int64_t fire_ball_size;
	/**
	 * True if ready to fire fire_ball, false otherwise
	 */
	bool ready_to_attack;
public:
	Magician (
	    act_id_t id,
	    PlayerId player_id,
	    int64_t attack,
	    int64_t hp,
	    int64_t max_hp,
	    int64_t max_speed,
	    int64_t size,
	    int64_t total_respawn_time,
	    float time_to_respawn,
	    int64_t time_spent_near_base,
	    physics::Vector2D position,
	    physics::Vector2D velocity,
	    int64_t los_radius,
	    int64_t attack_speed,
	    int64_t range,
	    int64_t fire_ball_speed,
	    int64_t fire_ball_ttl,
	    int64_t fire_ball_size
	);
	/**
	 * The magician's attack function
	 * Sets the ready_to_attack bool to true
	 */
	void Attack() override;
	/**
	 * The magician's StopAttack function
	 *
	 * Is called when target is dead
	 * Sets ready_to_attack bool to false
	 * Sets attack_target to nullptr
	 */
	void StopAttack() override;
	/**
	 * Determines if ready to attack.
	 * Getter for ready_to_attack
	 *
	 * @return     True if ready to attack, False otherwise.
	 */
	bool IsReadyToAttack();
	/**
	 * Gets a fire_ball's lifetime
	 *
	 * @return     The time to live for the fire_ball
	 */
	int64_t GetFireBallTtl();
	/**
	 * Gets the speed of a fire_ball fired by the magician
	 *
	 * @return     The fire_ball speed
	 */
	int64_t GetFireBallSpeed();
	/**
	 * Gets the size of a fire_ball fired from the magician
	 *
	 * @return     The fire_ball size
	 */
	int64_t GetFireBallSize();
	/**
	 * Sets ready_to_attack to false
	 */
	void SetReadyToAttackToFalse();
	/**
	 * Update function to be called every tick
	 */
	void Update(float delta_time) override;
};

}

#endif
