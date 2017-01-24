#ifndef PLAYER1_PLAYER1_H
#define PLAYER1_PLAYER1_H

#include <memory>
#include "player_ai_helper.h"
#include "player1_export.h"

namespace player1 {

class PLAYER1_EXPORT Player1 : public player::PlayerAiHelper {
public:
	void Update(std::shared_ptr<state::PlayerStateHandler> state) override;
};

}

#endif
