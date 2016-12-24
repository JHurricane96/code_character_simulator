/**
 * @file tower.h
 * Defines the Tower class
 */

#ifndef STATE_ACTOR_TOWER_H
#define STATE_ACTOR_TOWER_H

#include <cstdint>
#include "actor/actor.h"
#include "state_export.h"

/**
 * Describes the possible owners of the tower
 */
enum class TowerOwner{
	UNOWNED,
	PLAYER1,
	PLAYER2,
};

namespace state {
/**
 * Class for Towers
 */
class STATE_EXPORT Tower: public Actor {
private:
	/**
	 * The tower's owner
	 */
	TowerOwner tower_owner;
public:
	Tower();
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
