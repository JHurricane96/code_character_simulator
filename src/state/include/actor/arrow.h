/**
 * @file arrow.h
 * Defines the Arrow class
 */

#ifndef STATE_ACTOR_ARROW_H
#define STATE_ACTOR_ARROW_H

#include "actor/actor.h"
#include "state_export.h"
#include "terrain/terrain_element.h"

namespace state {

/**
 * Damage mutipliers for various terrains
 * 0: PLAIN
 * 1: FOREST
 * 2: MOUNTAIN
 * Multiplier for mountain to plain would be multiplier[MOUNTAIN][PLAIN]
 */
const std::vector<std::vector<float>> DamageMultiplier({
	{ (float)1.0, (float)0.7, (float)1.3 },
	{ (float)0.7, (float)0.7, (float)0.7 },
	{ (float)1.3, (float)1.0, (float)1.0 }
});

class STATE_EXPORT Arrow: public Actor {
private:
	/**
	 * True if hit target(done attacking), false if still in motion
	 */
	bool is_done;
	/**
	 * Lifetime of an arrow
	 * Dies after `time_to_live` clock ticks
	 */
	int64_t time_to_live;
public:
	Arrow(
		act_id_t id,
		PlayerId player_id,
		int64_t attack,
		int64_t max_speed,
		int64_t time_to_live,
		int64_t size,
		physics::Vector2D start,
		Actor* target
	);
	/**
	 * To check if arrow has hit target
	 * Gets is_done
	 *
	 * @return     True if done, False otherwise
	 */
	bool IsDone();
	/**
	 * The arrow's attack function
	 *
	 * Sets the is_done bool to true
	 * Inflicts damage on attack_target
	 */
	void Attack() override;
	/**
	 * Update function to be called every tick
	 */
	void Update(int64_t delta_time);
};

}


#endif
