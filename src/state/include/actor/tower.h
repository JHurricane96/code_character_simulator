#ifndef STATE_ACTOR_TOWER_H
#define STATE_ACTOR_TOWER_H

#include "actor/actor.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Tower: public Actor {
public:
	Tower();
	void Update();
};

}

#endif
