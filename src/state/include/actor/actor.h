#ifndef STATE_ACTOR_ACTOR_H
#define STATE_ACTOR_ACTOR_H

#include <cstdint>
#include "vector2d.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Actor {
private:
	act_id_t id;
	int64_t attack;
	int64_t hp;
	int64_t speed;
	physics::Vector2D position;
	physics::Vector2D velocity;
	act_id_t attack_target_id;
public:
	virtual void Update() = 0;
	act_id_t GetId();
	int64_t GetHp();
	int64_t GetSpeed();
	act_id_t GetAttackTargetId();
	physics::Vector2D GetVelocity();
	physics::Vector2D GetPosition();
};

}

#endif
