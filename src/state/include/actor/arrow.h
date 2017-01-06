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
public:
	Arrow();
	/**
	 * Sets the arrow in motion toward a destination
	 *
	 * @param[in]  destination  The destination of the arrow
	 */
	void FireArrow(
		std::vector<physics::Vector2D> start,
		std::vector<physics::Vector2D> destination
		);
	/**
	 * Update function to be called every tick
	 */
	void Update();
};

}


#endif
