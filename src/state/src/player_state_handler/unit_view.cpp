#include "player_state_handler/unit_views.h"

namespace state {

PathPlannerHelperView::PathPlannerHelperView(
	PathPlannerHelper * path_planner_helper) {
	if (path_planner_helper) {
		is_path_planning = path_planner_helper->IsPathPlanning();
	}
	else {
		is_path_planning = false;
	}
}

bool PathPlannerHelperView::IsPathPlanning(){
	return is_path_planning;
}

EnemyUnitView::EnemyUnitView() {}

EnemyUnitView::EnemyUnitView(Actor * actor)
	: id(actor->GetId()),
	actor_type(actor->GetActorType()),
	can_attack(actor->CanAttack()),
	can_plan_path(actor->CanPathPlan()),
	hp(actor->GetHp()),
	size(actor->GetSize()),
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

bool EnemyUnitView::CanAttack() {
	return can_attack;
}

bool EnemyUnitView::CanPathPlan() {
	return can_plan_path;
}

int64_t EnemyUnitView::GetSize() {
	return size;
}

int64_t EnemyUnitView::GetHp() {
	return hp;
}

UnitView::UnitView()
	: path_planner_helper(nullptr) {}

UnitView::UnitView(Actor * actor)
	: id(actor->GetId()),
	actor_type(actor->GetActorType()),
	can_attack(actor->CanAttack()),
	can_plan_path(actor->CanPathPlan()),
	attack(actor->GetAttack()),
	hp(actor->GetHp()),
	max_hp(actor->GetMaxHp()),
	max_speed(actor->GetMaxSpeed()),
	speed(actor->GetSpeed()),
	size(actor->GetSize()),
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
	can_attack(other.can_attack),
	can_plan_path(other.can_plan_path),
	attack(other.attack),
	hp(other.hp),
	max_hp(other.max_hp),
	max_speed(other.max_speed),
	speed(other.speed),
	size(other.size),
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

int64_t UnitView::GetSize() {
	return size;
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

bool UnitView::CanAttack() {
	return can_attack;
}

bool UnitView::CanPathPlan() {
	return can_plan_path;
}

PathPlannerHelperView UnitView::GetPathPlannerHelper() {
	return path_planner_helper;
}

TowerView::TowerView(Tower * tower):
	UnitView(tower),
	fire_ball_speed(tower->GetFireBallSpeed()),
	fire_ball_ttl(tower->GetFireBallTtl()),
	contention_radius(tower->GetContentionRadius()),
	contention_score(tower->GetContentionScore()),
	max_contention_score(tower->GetMaxContentionScore()) {}

int64_t TowerView::GetFireBallSpeed() {
	return fire_ball_speed;
}

int64_t TowerView::GetFireBallTtl() {
	return fire_ball_ttl;
}

int64_t TowerView::GetContentionRadius() {
	return contention_radius;
}

float TowerView::GetContentionScore() {
	return contention_score;
}

int64_t TowerView::GetMaxContentionScore() {
	return max_contention_score;
}

EnemyTowerView::EnemyTowerView(Tower * tower):
	EnemyUnitView(tower),
	contention_radius(tower->GetContentionRadius()),
	contention_score(tower->GetContentionScore()),
	max_contention_score(tower->GetMaxContentionScore()) {}

int64_t EnemyTowerView::GetContentionRadius() {
	return contention_radius;
}

float EnemyTowerView::GetContentionScore() {
	return contention_score;
}

int64_t EnemyTowerView::GetMaxContentionScore() {
	return max_contention_score;
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

BaseView::BaseView(Base * base):
	UnitView(base),
	base_poisoning_radius(base->GetBasePoisoningRadius()),
	base_poisoning_threshold(base->GetBasePoisoningThreshold()) {}

int64_t BaseView::GetBasePoisoningThreshold() {
	return base_poisoning_threshold;
}

int64_t BaseView::GetBasePoisoningRadius() {
	return base_poisoning_radius;
}

EnemyBaseView::EnemyBaseView(Base * base):
	EnemyUnitView(base),
	base_poisoning_radius(base->GetBasePoisoningRadius()),
	base_poisoning_threshold(base->GetBasePoisoningThreshold()) {}

int64_t EnemyBaseView::GetBasePoisoningThreshold() {
	return base_poisoning_threshold;
}

int64_t EnemyBaseView::GetBasePoisoningRadius() {
	return base_poisoning_radius;
}

KingView::KingView() {}

KingView::KingView(King * king):
	UnitView(king),
	has_flag(king->HasFlag()) {}

bool KingView::HasFlag() {
	return has_flag;
}

EnemyKingView::EnemyKingView() {}

EnemyKingView::EnemyKingView(King* king):
	EnemyUnitView(king),
	has_flag(king->HasFlag()) {}

bool EnemyKingView::HasFlag() {
	return has_flag;
}

}
