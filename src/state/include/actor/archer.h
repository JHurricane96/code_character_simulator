/**
 * @file archer.h
 * Defines the Archer class
 */

#ifndef STATE_ACTOR_ARCHER_H
#define STATE_ACTOR_ARCHER_H

#include <memory>
#include "actor/actor.h"
#include "actor/arrow.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {
/**
 * Class for Archers
 */
class STATE_EXPORT Archer: public Actor {
private:
	/**
	 * Speed of an arrow fired by the archer
	 */
	int64_t arrow_speed;
	/**
	 * Time to live for the arrows fired
	 */
	int64_t arrow_ttl;
	/**
	 * Size of the arrows fired
	 */
	int64_t arrow_size;
	/**
	 * True if ready to fire arrow, false otherwise
	 */
	bool ready_to_attack;
public:
	Archer (
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
	    int64_t arrow_speed,
	    int64_t arrow_ttl,
	    int64_t arrow_size
	);
	/**
	 * The archer's attack function
	 * Sets the ready_to_attack bool to true
	 */
	void Attack() override;
	/**
	 * The archer's StopAttack function
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
	 * Gets an arrow's lifetime
	 *
	 * @return     The time to live for the arrow
	 */
	int64_t GetArrowTtl();
	/**
	 * Gets the speed of an arrow fired by the archer
	 *
	 * @return     The arrow speed
	 */
	int64_t GetArrowSpeed();
	/**
	 * Gets the size of an arrow fired from the archer
	 *
	 * @return     The arrow size
	 */
	int64_t GetArrowSize();
	/**
	 * Sets ready_to_attack to false
	 */
	void SetReadyToAttackToFalse();
	/**
	 * Update function to be called every tick
	 */
	void Update(float delta_time);
};

}

#endif
