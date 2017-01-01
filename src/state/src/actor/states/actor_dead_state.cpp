#include "actor/states/actor_dead_state.h"
#include "actor/states/actor_idle_state.h"
#include "actor/actor.h"

namespace state {

void ActorDeadState::Enter(Actor * actor) {
	actor->Die();
}

std::unique_ptr<ActorState> ActorDeadState::Update(
	Actor * actor,
	int64_t delta_time
) {
	return nullptr;
}

void ActorDeadState::Exit(Actor * actor) {}

}
