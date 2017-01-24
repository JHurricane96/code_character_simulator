#ifndef PLAYER2_PLAYER2_H
#define PLAYER2_PLAYER2_H

#include <memory>
#include "player_ai_helper.h"
#include "player2_export.h"

namespace player2 {

class PLAYER2_EXPORT Player2 : public player::PlayerAiHelper {
public:
	void Update(std::shared_ptr<state::PlayerStateHandler> state) override;
};

}

#endif
