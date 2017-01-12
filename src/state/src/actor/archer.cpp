#include "actor/archer.h"
#include <algorithm>

namespace state {

Archer::Archer(
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
	int64_t range,
	int64_t arrow_speed,
	int64_t arrow_ttl,
	int64_t arrow_size
	) :
	ready_to_attack(false),
	arrow_speed(arrow_speed),
	arrow_ttl(arrow_ttl),
	arrow_size(arrow_size),
	Actor(
		id,
		player_id,
		attack,
		hp,
		max_hp,
		max_speed,
		size,
		total_respawn_time,
		time_to_respawn,
		time_spent_near_base,
		position,
		velocity,
		los_radius,
		attack_speed,
		range,
		ActorType::ARCHER
	) {}

void Archer::Attack() {
	ready_to_attack = true;
}

void Archer::StopAttack() {
	ready_to_attack = false;
	attack_target = nullptr;
}

bool Archer::IsReadyToAttack() {
	return ready_to_attack;
}

int64_t Archer::GetArrowTtl() {
	return arrow_ttl;
}

int64_t Archer::GetArrowSize() {
	return arrow_size;
}

int64_t Archer::GetArrowSpeed() {
	return arrow_speed;
}

void Archer::SetReadyToAttackToFalse() {
	ready_to_attack = false;
}

void Archer::Update(float delta_time) {
	DecideState(delta_time);
	position = position + velocity * delta_time;
};

}
