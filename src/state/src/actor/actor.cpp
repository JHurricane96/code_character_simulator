#include <memory>
#include "actor/actor.h"

namespace state {

PathPlannerHelper* Actor::GetPathPlannerHelper() {
	return &path_planner_helper;
}

Actor::Actor() : path_planner_helper(std::shared_ptr<Actor>(this)) {}

act_id_t Actor::GetId() {
	return id;
}

PlayerId Actor::GetPlayerId() {
	return player_id;
}

int64_t Actor::GetHp() {
	return hp;
}

int64_t Actor::GetMaxHp() {
	return max_hp;
}

int64_t Actor::GetMaxSpeed() {
	return max_speed;
}

int64_t Actor::GetSize() {
	return size;
}

int64_t Actor::GetTimeToRespawn() {
	return time_to_respawn;
}

act_id_t Actor::GetAttackTargetId() {
	return attack_target_id;
}

physics::Vector2D Actor::GetVelocity() {
	return velocity;
}

void Actor::SetVelocity(physics::Vector2D new_velocity) {
	velocity = new_velocity;
}

physics::Vector2D Actor::GetPosition() {
	return position;
}

}
