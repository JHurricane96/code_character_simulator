#ifndef PLAYER_PLAYER_AI
#define PLAYER_PLAYER_AI

#include "player_ai.h"

namespace player {

PlayerAi::PlayerAi(std::shared_ptr<player::PlayerAiHelper> helper) :
	helper(helper) {}

void PlayerAi::Update(std::shared_ptr<state::PlayerStateHandler> state) {
	helper->Update(state);
}

}

#endif
