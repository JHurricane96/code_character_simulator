/**
 * @file fire_ball.h
 * Defines the FireBall class
 */

#ifndef STATE_ACTOR_FIREBALL_H
#define STATE_ACTOR_FIREBALL_H

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

class STATE_EXPORT FireBall: public Actor {
private:
	/**
	 * True if hit target(done attacking), false if still in motion
	 */
	bool is_done;
	/**
	 * Lifetime of a fire_ball
	 * Dies after `time_to_live` clock ticks
	 */
	float time_to_live;
public:
	FireBall(
		act_id_t id,
		PlayerId player_id,
		int64_t attack,
		int64_t max_speed,
		float time_to_live,
		int64_t size,
		physics::Vector2D start,
		Actor* target
	);
	/**
	 * To check if fire_ball has hit target
	 * Gets is_done
	 *
	 * @return     True if done, False otherwise
	 */
	bool IsDone();
	/**
	 * The fire_ball's attack function
	 *
	 * Sets the is_done bool to true
	 * Inflicts damage on attack_target
	 */
	void Attack() override;
	/**
	 * Update function to be called every tick
	 */
	void Update(float delta_time) override;
	/**
	 * Merges this, a FireBall in the player's state, with the
	 * corresponding FireBall in the main state
	 *
	 * @param[in]  fire_ball   The player's FireBall
	 * @param[in]  actors      The player's state's Actors. Any
	 *                         references to the other state's Actors
	 *                         are replaced by this state's Actors
	 */
	void MergeWithMain(
		const FireBall * fire_ball,
		std::vector<std::shared_ptr<Actor> > actors
	);
};

}


#endif
