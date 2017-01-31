#include "player_state_handler/unit_views.h"

namespace state {

PathPlannerHelperView::PathPlannerHelperView(
	PathPlannerHelper * path_planner_helper):
	is_path_planning(path_planner_helper->IsPathPlanning()) {}

bool PathPlannerHelperView::IsPathPlanning(){
	return is_path_planning;
}

EnemyUnitView::EnemyUnitView() {}

EnemyUnitView::EnemyUnitView(Actor * actor)
	: id(actor->GetId()),
	actor_type(actor->GetActorType()),
	hp(actor->GetHp()),
	position(actor->GetPosition()) {}

act_id_t EnemyUnitView::GetId() {
	return id;
}

physics::Vector2D EnemyUnitView::GetPosition() {
	return position;
}

ActorType EnemyUnitView::GetActorType() {
	return actor_type;
}

int64_t EnemyUnitView::GetHp() {
	return hp;
}

UnitView::UnitView()
	: path_planner_helper(nullptr) {}

UnitView::UnitView(Actor * actor)
	: id(actor->GetId()),
	actor_type(actor->GetActorType()),
	attack(actor->GetAttack()),
	hp(actor->GetHp()),
	max_hp(actor->GetMaxHp()),
	max_speed(actor->GetMaxSpeed()),
	speed(actor->GetSpeed()),
	position(actor->GetPosition()),
	velocity(actor->GetVelocity()),
	los_radius(actor->GetLosRadius()),
	attack_range(actor->GetAttackRange()),
	path_planner_helper(PathPlannerHelperView(actor->GetPathPlannerHelper())
	) {
		auto target = actor->GetAttackTarget();
		if (target == nullptr) {
			attack_target = nullptr;
		}
		else {
			attack_target =
				std::unique_ptr<EnemyUnitView>(new EnemyUnitView(target));
		}
	}

UnitView::UnitView(const UnitView& other):
	id(other.id),
	actor_type(other.actor_type),
	attack(other.attack),
	hp(other.hp),
	max_hp(other.max_hp),
	max_speed(other.max_speed),
	speed(other.speed),
	position(other.position),
	velocity(other.velocity),
	los_radius(other.los_radius),
	attack_range(other.attack_range),
	path_planner_helper(other.path_planner_helper) {
		if (other.attack_target == nullptr) {
			attack_target = nullptr;
		}
		else {
			attack_target = std::unique_ptr<EnemyUnitView>(
				new EnemyUnitView(*other.attack_target)
			);
		}
	}

act_id_t UnitView::GetId() {
	return id;
}

int64_t UnitView::GetHp() {
	return hp;
}

int64_t UnitView::GetMaxHp() {
	return max_hp;
}

int64_t UnitView::GetMaxSpeed() {
	return max_speed;
}

EnemyUnitView * UnitView::GetAttackTarget(int * success) {
	if (success) {
		if (attack_target != nullptr) *success = 1;
		else *success = 0;
	}
	return attack_target.get();
}

physics::Vector2D UnitView::GetVelocity() {
	return velocity;
}

int64_t UnitView::GetAttackRange() {
	return attack_range;
}

physics::Vector2D UnitView::GetPosition() {
	return position;
}

ActorType UnitView::GetActorType() {
	return actor_type;
}

PathPlannerHelperView UnitView::GetPathPlannerHelper() {
	return path_planner_helper;
}

TowerView::TowerView(Tower * tower):
	UnitView(tower),
	fire_ball_speed(tower->GetFireBallSpeed()),
	fire_ball_ttl(tower->GetFireBallTtl()),
	contention_radius(tower->GetContentionRadius()) {}

int64_t TowerView::GetFireBallSpeed() {
	return fire_ball_speed;
}

int64_t TowerView::GetFireBallTtl() {
	return fire_ball_ttl;
}

int64_t TowerView::GetContentionRadius() {
	return contention_radius;
}

EnemyTowerView::EnemyTowerView(Tower * tower):
	EnemyUnitView(tower),
	contention_radius(tower->GetContentionRadius()) {}

int64_t EnemyTowerView::GetContentionRadius() {
	return contention_radius;
}

MagicianView::MagicianView(Magician * magician):
	UnitView(magician),
	fire_ball_speed(magician->GetFireBallSpeed()),
	fire_ball_ttl(magician->GetFireBallTtl()) {}

int64_t MagicianView::GetFireBallSpeed() {
	return fire_ball_speed;
}

int64_t MagicianView::GetFireBallTtl() {
	return fire_ball_ttl;
}

}
