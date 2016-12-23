/**
 * @file king.h
 * Defines the King class
 */

#ifndef STATE_ACTOR_KING_H
#define STATE_ACTOR_KING_H

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
	 */
	void Update();
};

}

#endif
