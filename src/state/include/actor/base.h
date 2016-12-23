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
	Base();
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
