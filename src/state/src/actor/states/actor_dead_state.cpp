#include "actor/states/actor_dead_state.h"
#include "actor/states/actor_idle_state.h"
#include "actor/actor.h"

namespace state {

void ActorDeadState::Enter(Actor * actor) {
	actor->Die();
}

std::unique_ptr<ActorState> ActorDeadState::Update(
	Actor * actor,
	float delta_time
) {
	if (actor->GetActorType() == ActorType::TOWER) {
		if (actor->IsDead()) {
			return nullptr;
		}
		else {
			return std::unique_ptr<ActorState>(new ActorIdleState());
		}
	}
	if (actor->GetTimeToRespawn() <= 0 &&
		actor->GetRespawnLocation() != nullptr) {
		return std::unique_ptr<ActorState>(new ActorIdleState());
	}
	actor->DecreaseRespawnTime(delta_time);
	return nullptr;
}

void ActorDeadState::Exit(Actor * actor) {
	if (actor->GetActorType() != ActorType::TOWER) {
		actor->Respawn();
	}
}

std::unique_ptr<ActorState> ActorDeadState::Clone() {
	return std::unique_ptr<ActorState>(new ActorDeadState(*this));
}

}
