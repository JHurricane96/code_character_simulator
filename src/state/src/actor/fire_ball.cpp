#include "actor/fire_ball.h"

namespace state {

FireBall::FireBall(
	act_id_t id,
	PlayerId player_id,
	int64_t attack,
	int64_t max_speed,
	float time_to_live,
	int64_t size,
	physics::Vector2D start,
	Actor* target
	) :
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
		ActorType::FIREBALL,
		false,
		false
	),
	is_done(false),
	time_to_live(time_to_live) {
		attack_target = target;
	}

bool FireBall::IsDone() {
	return is_done;
}

void FireBall::Attack() {
	is_done = true;
	attack_target->Damage(attack);
}

void FireBall::Update(float delta_time) {
	time_to_live -= delta_time;
	if (time_to_live < 0 || attack_target->IsDead()) {
		is_done = true;
		return;
	}

	if (position.distance(attack_target->GetPosition())
		< size + attack_target->GetSize())
	Attack();

	auto to_target = attack_target->GetPosition() - position;
	velocity = to_target * max_speed / to_target.magnitude();
	position = position + velocity * delta_time;
};

void FireBall::MergeWithMain(
	const FireBall * fire_ball,
	std::vector<std::shared_ptr<Actor> > actors
) {
	Actor::MergeWithMain(fire_ball, actors);
}

}
