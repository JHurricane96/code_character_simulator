#include <algorithm>
#include <memory>
#include "actor/actor.h"
#include "actor/states/actor_idle_state.h"

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
		float time_to_respawn,
		int64_t time_spent_near_base,
		physics::Vector2D position,
		physics::Vector2D velocity,
		int64_t los_radius,
		int64_t attack_speed,
		int64_t attack_range,
		ActorType actor_type,
		bool can_attack,
		bool can_plan_path
	):
	id(id),
	player_id(player_id),
	actor_type(actor_type),
	state(new ActorIdleState()),
	can_attack(can_attack),
	can_plan_path(can_plan_path),
	attack(attack),
	hp(hp),
	max_hp(max_hp),
	max_speed(max_speed),
	speed(max_speed),
	size(size),
	total_respawn_time(total_respawn_time),
	time_to_respawn(time_to_respawn),
	time_spent_near_base(time_spent_near_base),
	position(position),
	velocity(velocity),
	attack_target(nullptr),
	los_radius(los_radius),
	attack_speed(attack_speed),
	attack_range(attack_range),
	is_dead(false),
	respawn_location(nullptr),
	path_planner_helper() {}

Actor::Actor(const Actor& other) {
	id = other.id;
	player_id = other.player_id;
	actor_type = other.actor_type;
	state = other.state->Clone();
	attack = other.attack;
	hp = other.hp;
	max_hp = other.max_hp;
	max_speed = other.max_speed;
	size = other.size;
	total_respawn_time = other.total_respawn_time;
	time_to_respawn = other.time_to_respawn;
	time_spent_near_base = other.time_spent_near_base;
	position = other.position;
	velocity = other.velocity;
	attack_target = other.attack_target;
	los_radius = other.los_radius;
	attack_speed = other.attack_speed;
	attack_range = other.attack_range;
	is_dead = other.is_dead;
	respawn_location = other.respawn_location;
	path_planner_helper = other.path_planner_helper;
}

void Actor::AddPathPlanner(PathPlannerHelper p) {
	path_planner_helper = p;
}

act_id_t Actor::GetId() {
	return id;
}

PlayerId Actor::GetPlayerId() {
	return player_id;
}

ActorType Actor::GetActorType() {
	return actor_type;
}

int64_t Actor::CanAttack() {
	return can_attack;
}

int64_t Actor::CanPathPlan() {
	return can_plan_path;
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

float Actor::GetSpeed() {
	return speed;
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

int64_t Actor::GetAttackRange() {
	return attack_range;
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

void Actor::SetSpeed(float speed) {
	this->speed = speed;
}

void Actor::Die() {
	is_dead = true;
	time_to_respawn = total_respawn_time;
	hp = 0;
	time_spent_near_base = 0;
	position.x = position.y = 0;
	respawn_location = nullptr;
	attack_target = nullptr;
}

void Actor::Respawn() {
	is_dead = false;
	hp = max_hp;
	position = respawn_location->GetPosition();
}

void Actor::DecreaseRespawnTime(float delta_time) {
	time_to_respawn = std::max((int64_t) 0, (int64_t) (time_to_respawn - delta_time));
}

void Actor::DecideState(float delta_time) {
	auto new_state = state->Update(this, delta_time);

	while (new_state != nullptr) {
		state->Exit(this);
		state = std::move(new_state);
		state->Enter(this);
		new_state = state->Update(this, delta_time);
	}
}

void Actor::AttackUnit(Actor * target) {
	attack_target = target;
}

void Actor::Attack() {}

void Actor::StopAttack() {
	attack_target = nullptr;
}

void Actor::Damage(int64_t damage_amount) {
	hp -= damage_amount;
}

Actor * Actor::GetRespawnLocation() {
	return respawn_location;
}

void Actor::SetRespawnLocation(Actor * respawn_location) {
	this->respawn_location = respawn_location;
}

void Actor::CheckBounds(physics::Vector2D bounds) {
	if (position.x <= 0) {
		position.x = 0;
	}
	if (position.y <= 0) {
		position.y = 0;
	}
	if (position.x >= bounds.x) {
		position.x = bounds.x - 1;
	}
	if (position.y >= bounds.y) {
		position.y = bounds.y - 1;
	}
}

void Actor::MergeWithBuffer(
	const Actor * actor,
	std::vector<std::shared_ptr<Actor> > actors
) {
	speed = actor->speed;
	velocity = actor->velocity;
	path_planner_helper = actor->path_planner_helper;
	path_planner_helper.MergeWithBuffer(
		actor->path_planner_helper,
		actors
	);
	if (actor->attack_target != nullptr) {
		attack_target = actors[actor->attack_target->id].get();
	}
	else {
		attack_target = nullptr;
	}
	if (actor->respawn_location != nullptr) {
		respawn_location = actors[actor->respawn_location->id].get();
	}
	else {
		respawn_location = nullptr;
	}
}

void Actor::MergeWithMain(
	const Actor * actor,
	std::vector<std::shared_ptr<Actor> > actors
) {
	player_id = actor->player_id;
	state = actor->state->Clone();
	hp = actor->hp;
	time_to_respawn = actor->time_to_respawn;
	time_spent_near_base = actor->time_spent_near_base;
	position = actor->position;
	is_dead = actor->is_dead;
	MergeWithBuffer(actor, actors);
}

}
