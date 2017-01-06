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
public:
	Arrow();
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
	void Update();
};

}


#endif
