#include "actor/flag.h"
#include <vector>
#include <memory>
#include "actor/king.h"

namespace state {

Flag::Flag(
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
		attack_speed,
		0,
		ActorType::FLAG,
		false,
		false
	), king(nullptr) {}

void Flag::Capture(King * king) {
	this->king = king;
}

void Flag::Drop() {
	king = nullptr;
}

bool Flag::IsCaptured() {
	return (king != nullptr);
}

void Flag::MoveToBase(physics::Vector2D base_position) {
	position = base_position;
}

void Flag::Update(float delta_time) {
	if (IsCaptured()) {
		position = king->GetPosition();
	}
}

void Flag::MergeWithBuffer(
	const Flag * flag,
	std::vector<std::shared_ptr<Actor> > actors
) {
	if (flag->king != nullptr) {
		king = static_cast<King *>(actors[flag->king->GetId()].get());

	}
	else {
		if (king != nullptr) {
			position = flag->position;
		}
		king = nullptr;
	}
}

void Flag::MergeWithMain(
	const Flag * flag,
	std::vector<std::shared_ptr<Actor> > actors
) {
	MergeWithBuffer(flag, actors);
}

}
