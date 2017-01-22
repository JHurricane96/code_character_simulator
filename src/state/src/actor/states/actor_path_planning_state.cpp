#include "actor/states/actor_path_planning_state.h"
#include "actor/states/actor_attack_state.h"
#include "actor/states/actor_dead_state.h"
#include "actor/states/actor_idle_state.h"
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"

namespace state {

void ActorPathPlanningState::Enter(Actor * actor) {}

std::unique_ptr<ActorState> ActorPathPlanningState::Update(
	Actor * actor,
	float delta_time
) {
	if (actor->GetHp() <= 0) {
		return std::unique_ptr<ActorState>(new ActorDeadState());
	}
	else if (actor->GetAttackTarget() != nullptr) {
		return std::unique_ptr<ActorState>(new ActorAttackState());
	}
	else if (!actor->GetPathPlannerHelper()->IsPathPlanning()) {
		return std::unique_ptr<ActorState>(new ActorIdleState());
	}

	auto to_dest = actor->GetPathPlannerHelper()->GetDestination() -
		actor->GetPosition();
	if (to_dest.magnitude() <
		(actor->GetVelocity() * delta_time).magnitude()) {
		actor->SetVelocity(to_dest / delta_time);
	}
	return nullptr;
}

void ActorPathPlanningState::Exit(Actor * actor) {
	actor->GetPathPlannerHelper()->FinishPath();
}

std::unique_ptr<ActorState> ActorPathPlanningState::Clone() {
	return std::unique_ptr<ActorState>(
		new ActorPathPlanningState(*this)
	);
}

}
