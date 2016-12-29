#include "actor/king.h"
#include "actor/flag.h"

namespace state {

King::King(
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
		attack_speed
	), flag(nullptr) {}

void King::CaptureFlag(Flag * flag) {
	this->flag = flag;
}

void King::DropFlag() {
	flag = nullptr;
}

bool King::HasFlag() {
	return (flag != nullptr);
}

void King::Update(int64_t delta_time) {
	position = position + velocity * delta_time;
}


}
