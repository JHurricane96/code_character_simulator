#ifndef PLAYER_PLAYER_AI_HELPER_H
#define PLAYER_PLAYER_AI_HELPER_H

#include <memory>

#include "player_state_handler/player_state_handler.h"
#include "player_export.h"

namespace player {

/**
 * Class where player defines AI code
 */
class PLAYER_EXPORT PlayerAiHelper {
public:
	/**
	 * Player AI update function (main logic of the AI)
	 */
	virtual void Update(std::shared_ptr<state::PlayerStateHandler> state) = 0;
};

}

#endif
