/**
 * @file tower.h
 * Defines the Tower class
 */

#ifndef STATE_ACTOR_TOWER_H
#define STATE_ACTOR_TOWER_H

#include "actor/actor.h"
#include "state_export.h"

/**
 * Describes the possible owners of the tower
 */
enum TOWER_OWNER{
	UNOWNED,
	PLAYER1,
	PLAYER2,
}

namespace state {
/**
 * Class for Towers
 */
class STATE_EXPORT Tower: public Actor {
private:
	/**
	 * The tower's owner
	 */
	TOWER_OWNER tower_owner;
public:
	Tower();
	/**
	 * Update function to be called every tick
	 */
	void Update();
};

}

#endif
