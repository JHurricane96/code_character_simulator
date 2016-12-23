/**
 * @file king.h
 * Defines the King class
 */

#ifndef STATE_ACTOR_KING_H
#define STATE_ACTOR_KING_H

#include <cstdint>
#include "actor/actor.h"
#include "state_export.h"

namespace state {
/**
 * Class for the king
 *
 * The king is the only unit that can captue a flag and carry it
 */
class STATE_EXPORT King: public Actor {
public:
	King();
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
