/**
 * @file arrow.h
 * Defines the Flag Class
 */

#ifndef STATE_ACTOR_FLAG_H
#define STATE_ACTOR_FLAG_H

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
	 */
	void Update();
};

}


#endif
