/**
 * @file flag.h
 * Defines the Flag Class
 */

#ifndef STATE_ACTOR_FLAG_H
#define STATE_ACTOR_FLAG_H

#include <cstdint>
#include "actor/actor.h"
#include "state_export.h"

namespace state {
/**
 * Class for Flags that the players try to capture
 */
class STATE_EXPORT Flag: public Actor {
public:
	Flag(
		act_id_t id,
		PlayerId player_id,
		int64_t attack,
		int64_t hp,
		int64_t max_hp,
		int64_t max_speed,
		int64_t size,
		int64_t total_respawn_time,
		int64_t time_to_respawn,
		int64_t time_spent_near_base,
		physics::Vector2D position,
		physics::Vector2D velocity,
		int64_t los_radius
	);
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(int64_t delta_time);
};

}


#endif
