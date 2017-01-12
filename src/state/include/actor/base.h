/**
 * @file base.h
 * Defines the (Player)Base Class
 */

#ifndef STATE_ACTOR_BASE_H
#define STATE_ACTOR_BASE_H

#include <cstdint>
#include "actor/actor.h"
#include "state_export.h"

namespace state {
/**
 * Class that defines the players' base where the flag is initially
 * present and troops spawn
 */
class STATE_EXPORT Base: public Actor {
public:
	Base(
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
		int64_t attack_speed
	);
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(float delta_time);
};

}


#endif
