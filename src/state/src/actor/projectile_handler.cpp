#include "actor/projectile_handler.h"
#include <algorithm>

namespace state {

ProjectileHandler::ProjectileHandler() {}

ProjectileHandler::ProjectileHandler(
	int64_t first_fire_ball_id
	) :
	fire_balls(),
	next_fire_ball_id(first_fire_ball_id) {}

void ProjectileHandler::CreateFireBall(
	PlayerId player_id,
	physics::Vector2D start,
	Actor* attack_target,
	int64_t max_speed,
	int64_t ttl,
	int64_t size,
	int64_t damage
) {
	std::shared_ptr<FireBall> fire_ball(
		new FireBall(
			next_fire_ball_id,
			player_id,
			damage,
			max_speed,
			ttl,
			size,
			start,
			attack_target));
	fire_balls.push_back(fire_ball);
	next_fire_ball_id++;
}


std::vector<std::shared_ptr<FireBall> >
ProjectileHandler::GetProjectiles() {
	return fire_balls;
}

template <typename ActorType>
void ProjectileHandler::HandleActors(
	std::vector<std::vector<std::shared_ptr<ActorType> > >& actors,
	Terrain* terrain
) {
	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		for (auto actor : actors[pid]) {
			if (actor->IsReadyToAttack()) {
				auto target = actor->GetAttackTarget();
				auto damage = actor->GetAttack();
				auto actor_pos = actor->GetPosition();
				auto fire_ball_dest = target->GetPosition();
				terrain->CoordinateToTerrainElement(actor_pos).GetTerrainType();
				auto tot_damage = damage *
				  DamageMultiplier [terrain->CoordinateToTerrainElement(actor_pos)
										.GetTerrainType()]
								   [terrain->CoordinateToTerrainElement(fire_ball_dest)
								   		.GetTerrainType()];
				CreateFireBall(
					actor->GetPlayerId(),
					actor_pos,
					target,
					actor->GetFireBallSpeed(),
					actor->GetFireBallTtl(),
					actor->GetFireBallSize(),
					tot_damage);
				actor->SetReadyToAttackToFalse();
			}
		}
	}
}

void ProjectileHandler::HandleFireBalls(
	float delta_time
) {
	std::vector<std::shared_ptr<FireBall> > new_fire_balls;
	for (int64_t i = 0; i < fire_balls.size(); i++) {
		fire_balls[i]->Update(delta_time);
		if (!fire_balls[i]->IsDone()) {
			new_fire_balls.push_back(fire_balls[i]);
		}
	}
	fire_balls = new_fire_balls;
}

void ProjectileHandler::Update(
	float delta_time,
	std::vector<std::vector<std::shared_ptr<Tower> > >& towers,
	std::vector<std::vector<std::shared_ptr<Magician> > >& magicians,
	Terrain* terrain
) {
	HandleActors<Tower>(towers, terrain);
	HandleActors<Magician>(magicians, terrain);
	HandleFireBalls(delta_time);
}

void ProjectileHandler::MergeWithMain(
	const ProjectileHandler& proj_handler,
	std::vector<std::shared_ptr<Actor> > actors
) {
	fire_balls.clear();
	for (auto fire_ball : proj_handler.fire_balls) {
		fire_balls.push_back(
			std::shared_ptr<FireBall>(new FireBall(*fire_ball))
		);
		fire_balls.back()->MergeWithMain(fire_ball.get(), actors);
	}
	next_fire_ball_id = proj_handler.next_fire_ball_id;
}

}
