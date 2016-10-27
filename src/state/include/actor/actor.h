#ifndef STATE_ACTOR_ACTOR_H
#define STATE_ACTOR_ACTOR_H

#include <cstdint>
#include "vector2d.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Actor {
private:
	int64_t id;
	int64_t attack;
	int64_t hp;
	int64_t speed;
	physics::Vector2D position;
	physics::Vector2D velocity;
	int64_t attack_target_id;
public:
	virtual void Update() = 0;
	int64_t GetId();
	int64_t GetHp();
	int64_t GetSpeed();
	int64_t GetAttackTargetId();
	physics::Vector2D GetVelocity();
	physics::Vector2D GetPosition();
};

}

#endif
