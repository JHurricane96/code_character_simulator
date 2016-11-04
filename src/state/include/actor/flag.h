#ifndef STATE_ACTOR_FLAG_H
#define STATE_ACTOR_FLAG_H

#include "actor/actor.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Flag: public Actor {
public:
	Flag();
    /**
     * @brief      Update function to be called every tick
     */
	void Update();
};

}


#endif
