#ifndef STATE_ACTOR_TOWER_H
#define STATE_ACTOR_TOWER_H

#include "actor/actor.h"
#include "state_export.h"

namespace state {
/**
 * @brief      Class for Towers
 */
class STATE_EXPORT Tower: public Actor {
private:
    /**
     * A boolean to indicate if the tower is captured by a player
     */
	bool captured;
    /**
     * ID of the player owning if the tower if captured
     */
	act_id_t owner_id;
public:
	Tower();
    /**
     * @brief      Update function to be called every tick
     */
	void Update();
};

}

#endif
