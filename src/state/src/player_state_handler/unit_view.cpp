#include "player_state_handler/unit_views.h"

namespace state {

PathPlannerHelperView::PathPlannerHelperView(
    PathPlannerHelper * path_planner_helper):
    path_planner_helper(path_planner_helper) {}

bool PathPlannerHelperView::IsPathPlanning(){
    return path_planner_helper->IsPathPlanning();
}

EnemyUnitView::EnemyUnitView(): unit(nullptr) {}

EnemyUnitView::EnemyUnitView(Actor * actor): unit(actor) {}

act_id_t EnemyUnitView::GetId() {
	return unit->GetId();
}

physics::Vector2D EnemyUnitView::GetPosition() {
	return unit->GetPosition();
}

ActorType EnemyUnitView::GetActorType() {
	return unit->GetActorType();
}

UnitView::UnitView(): unit(nullptr) {}

UnitView::UnitView(Actor * actor): unit(actor) {}

act_id_t UnitView::GetId() {
	return unit->GetId();
}

int64_t UnitView::GetHp() {
	return unit->GetHp();
}

int64_t UnitView::GetMaxHp() {
	return unit->GetMaxHp();
}

int64_t UnitView::GetMaxSpeed() {
	return unit->GetMaxSpeed();
}

EnemyUnitView UnitView::GetAttackTarget(int * success) {
	auto target =  unit->GetAttackTarget();
	if (success) {
		if (target != nullptr) *success = 1;
		else *success = 0;
	}
	return EnemyUnitView(target);
}

physics::Vector2D UnitView::GetVelocity() {
	return unit->GetVelocity();
}

int64_t UnitView::GetAttackRange() {
	return unit->GetAttackRange();
}

physics::Vector2D UnitView::GetPosition() {
	return unit->GetPosition();
}

ActorType UnitView::GetActorType() {
	return unit->GetActorType();
}

PathPlannerHelperView UnitView::GetPathPlannerHelper() {
	return PathPlannerHelperView(unit->GetPathPlannerHelper());
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
