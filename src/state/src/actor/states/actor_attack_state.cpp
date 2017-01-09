#include "actor/states/actor_attack_state.h"
#include "actor/states/actor_pursuit_state.h"
#include "actor/states/actor_path_planning_state.h"
#include "actor/states/actor_dead_state.h"
#include "actor/states/actor_idle_state.h"
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"

namespace state {

void ActorAttackState::Enter(Actor * actor) {
	time_to_attack = actor->GetAttackSpeed();
}

std::unique_ptr<ActorState> ActorAttackState::Update(
	Actor * actor,
	int64_t delta_time
) {
	auto target = actor->GetAttackTarget();
	if (actor->GetHp() <= 0) {
		actor->StopAttack();
		return std::unique_ptr<ActorState>(new ActorDeadState());
	}
	else if (actor->GetPathPlannerHelper()->IsPathPlanning()) {
		actor->StopAttack();
		return std::unique_ptr<ActorState>(
			new ActorPathPlanningState()
		);
	}
	else if (target == nullptr || target->IsDead()) {
		actor->StopAttack();
		return std::unique_ptr<ActorState>(new ActorIdleState());
	}
	else if (actor->GetPosition().distance(target->GetPosition())
		> actor->GetAttackRange() + target->GetSize()
	) {
		return std::unique_ptr<ActorState>(new ActorPursuitState());
	}

	time_to_attack -= delta_time;
	if (time_to_attack <= 0) {
		time_to_attack = actor->GetAttackSpeed();
		actor->Attack();
	}

	return nullptr;
}

void ActorAttackState::Exit(Actor * actor) {}

std::unique_ptr<ActorState> ActorAttackState::Clone() {
	return std::unique_ptr<ActorState>(new ActorAttackState(*this));
}

}
