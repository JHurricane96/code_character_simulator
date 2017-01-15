#ifndef PLAYER_PLAYER_AI_H
#define PLAYER_PLAYER_AI_H

#include <memory>

#include "state.h"
#include "player_ai_helper.h"
#include "player_state_handler/player_state_handler.h"

namespace player {

/**
 * Class for Player AI
 */
class PlayerAi {
	/**
	 * Helper object defined by the Player
	 */
	std::shared_ptr<player::PlayerAiHelper> helper;
public:
	PlayerAi(std::shared_ptr<player::PlayerAiHelper> helper);
	/**
	 * Function where the Player AI calls Player AI helper
	 *
	 * @param[in]  state  The Buffer State Handler used by the Player
	 */
	void Update(std::shared_ptr<state::PlayerStateHandler> state);
};

}

#endif
