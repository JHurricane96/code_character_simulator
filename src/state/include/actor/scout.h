/**
 * @file scout.h
 * Defines the Scout class
 */

#ifndef STATE_ACTOR_SCOUT_H
#define STATE_ACTOR_SCOUT_H

#include <memory>
#include "actor/actor.h"
#include "state_export.h"

namespace state {
/**
 * Class for scout
 *
 * The scout cannot attack and is invisible to the enemy unless he is
 * within the line of site of the base or a tower
 */
class STATE_EXPORT Scout: public Actor {
public:
	Scout(
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
		int64_t attack_range
	);
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(float delta_time) override;
};

}

#endif
