/**
 * @file swordsman.h
 * Defines the Swordsman class
 */

#ifndef STATE_ACTOR_SWORDSMAN_H
#define STATE_ACTOR_SWORDSMAN_H

#include <cstdint>
#include <memory>
#include "actor/actor.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Swordsman: public Actor {
public:
	Swordsman();
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
