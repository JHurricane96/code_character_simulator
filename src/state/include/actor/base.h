#ifndef STATE_ACTOR_BASE_H
#define STATE_ACTOR_BASE_H

#include "actor/actor.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Base: public Actor {
public:
	Base();
    /**
     * @brief      Update function to be called every tick
     */
	void Update();
};

}


#endif
