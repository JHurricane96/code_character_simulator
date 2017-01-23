#include "actor/base.h"

namespace state {

Base::Base(
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
		int64_t base_poisoning_radius,
		int64_t base_poisoning_threshold
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
		ActorType::BASE,
		false,
		false
	),
	base_poisoning_radius(base_poisoning_radius),
	base_poisoning_threshold(base_poisoning_threshold) {};

int64_t Base::GetBasePoisonPenalty(
	std::vector<std::shared_ptr<Actor> > actors
) {
	int64_t count_actors_near_base = 0;
	for (auto actor : actors) {
			if (actor->GetActorType() != ActorType::TOWER &&
			actor->GetActorType() != ActorType::BASE &&
			actor->GetActorType() != ActorType::FLAG &&
			actor->GetPosition().distance(position) <
			base_poisoning_radius) {
			count_actors_near_base++;
		}
	}
	int64_t penalty = count_actors_near_base - base_poisoning_threshold;
	return penalty;
}

void Base::Update(float delta_time) {};

}
