#include <memory>
#include "actor/actor.h"

namespace state {

PathPlannerHelper* Actor::GetPathPlannerHelper() {
	return &path_planner_helper;
}

Actor::Actor() : path_planner_helper(std::shared_ptr<Actor>(this)) {}
Actor::Actor(
		act_id_t id,
		PlayerId player_id,
		int64_t attack,
		int64_t hp,
		int64_t max_hp,
		int64_t max_speed,
		int64_t size,
		int64_t total_respawn_time,
		int64_t time_to_respawn,
		int64_t time_spent_near_base,
		physics::Vector2D position,
		physics::Vector2D velocity,
		int64_t los_radius,
		int64_t attack_speed
	):
	id(id),
	player_id(player_id),
	attack(attack),
	hp(hp),
	max_hp(max_hp),
	max_speed(max_speed),
	size(size),
	total_respawn_time(total_respawn_time),
	time_to_respawn(time_to_respawn),
	time_spent_near_base(time_spent_near_base),
	position(position),
	velocity(velocity),
	attack_target(nullptr),
	los_radius(los_radius),
	is_dead(false),
	attack_speed(attack_speed),
	path_planner_helper() {}

void Actor::AddPathPlanner(PathPlannerHelper p) {
	path_planner_helper = p;
}

act_id_t Actor::GetId() {
	return id;
}

PlayerId Actor::GetPlayerId() {
	return player_id;
}

int64_t Actor::GetAttack() {
	return attack;
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

Actor * Actor::GetAttackTarget() {
	return attack_target;
}

int64_t Actor::GetAttackSpeed() {
	return attack_speed;
}

physics::Vector2D Actor::GetVelocity() {
	return velocity;
}

int64_t Actor::GetLosRadius() {
	return los_radius;
}

void Actor::SetVelocity(physics::Vector2D new_velocity) {
	velocity = new_velocity;
}

physics::Vector2D Actor::GetPosition() {
	return position;
}

bool Actor::IsDead() {
	return is_dead;
}

void Actor::Die() {
	is_dead = true;
	time_to_respawn = total_respawn_time;
	hp = 0;
	time_spent_near_base = 0;
	position.x = position.y = 0;
	attack_target = nullptr;
}

void Actor::AttackUnit(Actor * target) {
	attack_target = target;
}

void Actor::StopAttack() {
	attack_target = nullptr;
}

void Actor::Damage(int64_t damage_amount) {
	hp -= damage_amount;
}

}
