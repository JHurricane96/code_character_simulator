#include "actor/magician.h"
#include <algorithm>

namespace state {

Magician::Magician(
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
	int64_t fire_ball_speed,
	int64_t fire_ball_ttl,
	int64_t fire_ball_size
	) :
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
		ActorType::MAGICIAN),
	fire_ball_speed(fire_ball_speed),
	fire_ball_ttl(fire_ball_ttl),
	fire_ball_size(fire_ball_size),
	ready_to_attack(false) {}

void Magician::Attack() {
	ready_to_attack = true;
}

void Magician::StopAttack() {
	ready_to_attack = false;
	attack_target = nullptr;
}

bool Magician::IsReadyToAttack() {
	return ready_to_attack;
}

int64_t Magician::GetFireBallTtl() {
	return fire_ball_ttl;
}

int64_t Magician::GetFireBallSize() {
	return fire_ball_size;
}

int64_t Magician::GetFireBallSpeed() {
	return fire_ball_speed;
}

void Magician::SetReadyToAttackToFalse() {
	ready_to_attack = false;
}

void Magician::Update(float delta_time) {
	DecideState(delta_time);
	position = position + velocity * delta_time;
};

}
