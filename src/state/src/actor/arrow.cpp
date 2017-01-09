#include "actor/arrow.h"

namespace state {

Arrow::Arrow(
	act_id_t id,
	PlayerId player_id,
	int64_t attack,
	int64_t max_speed,
	int64_t time_to_live,
	int64_t size,
	physics::Vector2D start,
	Actor* target
	) :
	is_done(false),
	time_to_live(time_to_live),
	Actor(
		id,
		player_id,
		attack,
		0,
		0,
		max_speed,
		size,
		0,
		0,
		0,
		start,
		physics::Vector2D(0, 0),
		0,
		0,
		size,
		ActorType::ARROW
	) {
		attack_target = target;
	}

bool Arrow::IsDone() {
	return is_done;
}

void Arrow::Attack() {
	is_done = true;
	attack_target->Damage(attack);
}

void Arrow::Update(int64_t delta_time) {
	time_to_live -= delta_time;
	if (time_to_live < 0 || attack_target->IsDead())
		is_done = true;

	if (position.distance(attack_target->GetPosition())
		< size + attack_target->GetSize())
	Attack();

	auto to_target = attack_target->GetPosition() - position;
	velocity = to_target * max_speed / to_target.magnitude();
	position = position + velocity * delta_time;
};

void Arrow::MergeWithMain(
	const Arrow * arrow,
	std::vector<std::shared_ptr<Actor> > actors
) {
	Actor::MergeWithMain(arrow, actors);
}

}
