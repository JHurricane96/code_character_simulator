#ifndef STATE_ACTOR_ACTOR_H
#define STATE_ACTOR_ACTOR_H

#include <cstdint>
#include "point.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Actor {
private:
	int64_t id;
	int64_t attack;
	int64_t hp;
	int64_t speed;
	point::Point position;
	point::Point velocity;
	int64_t attacking_which;
public:
	virtual void Update() = 0;
};

}

#endif
