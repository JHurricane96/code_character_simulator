#ifndef STATE_ACTOR_ARROW_H
#define STATE_ACTOR_ARROW_H

#include "actor/actor.h"
#include "state_export.h"

typedef multiplier float;

namespace state {

class STATE_EXPORT Arrow: public Actor {
private:
	/**
	 * The damage the arrow causes
	 */
	int64_t damage;
	/**
	 * Damage mutipliers for various terrains
     * TODO: Set sensible numbers
	 */
	const multiplier plane_plane		= 1;
	const multiplier plane_mountain		= 1;
	const multiplier plane_forest		= 1;
	const multiplier mountain_plane		= 1;
	const multiplier mountain_mountain	= 1;
	const multiplier mountain_forest	= 1;
	const multiplier forest_plane		= 1;
	const multiplier forest_mountain	= 1;
	const multiplier forest_forest		= 1;
public:
	Arrow();
	/**
	 * @brief      Sets the arrow in motion toward a destination
	 *
	 * @param[in]  destination  The destination of the arrow
	 */
	void FireArrow(
        std::vector<physics::Vector2D> start,
        std::vector<physics::Vector2D> destination
        );
    /**
     * @brief      Update function to be called every tick
     */
	void Update();
};

}


#endif
