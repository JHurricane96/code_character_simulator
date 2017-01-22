#include "actor/states/actor_pursuit_state.h"
#include "actor/states/actor_attack_state.h"
#include "actor/states/actor_path_planning_state.h"
#include "actor/states/actor_idle_state.h"
#include "actor/states/actor_dead_state.h"
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"

namespace state {

void ActorPursuitState::Enter(Actor * actor) {}

std::unique_ptr<ActorState> ActorPursuitState::Update(
	Actor * actor,
	float delta_time
) {
	auto target = actor->GetAttackTarget();
	if (actor->GetHp() <= 0) {
		actor->SetVelocity(physics::Vector2D(0, 0));
		return std::unique_ptr<ActorState>(new ActorDeadState());
	}
	else if (actor->GetPathPlannerHelper()->IsPathPlanning()) {
		actor->StopAttack();
		return std::unique_ptr<ActorState>(
			new ActorPathPlanningState()
		);
	}
	else if (target == nullptr || target->IsDead()) {
		actor->SetVelocity(physics::Vector2D(0, 0));
		actor->StopAttack();
		return std::unique_ptr<ActorState>(new ActorIdleState());
	}
	else if (actor->GetPosition().distance(target->GetPosition())
		<= actor->GetAttackRange() + target->GetSize()
	) {
		actor->SetVelocity(physics::Vector2D(0, 0));
		return std::unique_ptr<ActorState>(new ActorAttackState());
	}

	auto to_target = target->GetPosition() - actor->GetPosition();
	actor->SetVelocity(
		to_target * actor->GetSpeed() / to_target.magnitude()
	);
	return nullptr;
}

void ActorPursuitState::Exit(Actor * actor) {}

std::unique_ptr<ActorState> ActorPursuitState::Clone() {
	return std::unique_ptr<ActorState>(new ActorPursuitState(*this));
}

}
