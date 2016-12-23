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
	Flag();
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
