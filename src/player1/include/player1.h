#ifndef PLAYER1_PLAYER1_H
#define PLAYER1_PLAYER1_H

#include <memory>
#include "player_state_handler/player_state_handler.h"
#include "ipc.h"
#include "player_ai_helper.h"
#include "player1_export.h"

namespace player1 {

class PLAYER1_EXPORT Player1 : public player::PlayerAiHelper {
private:
	bool init = false;

	int64_t terrain_elt_size;

	state::list_act_id_t def_ids;
	int def_mode = 0;
	state::act_id_t def_att_target = -1;

	std::vector<physics::Vector2D> patrol_path;
	int cur_patrol_pnt = 0;

	state::FormationMaker* together_formation;
public:
	void Update(std::shared_ptr<state::PlayerStateHandler> state) override;
};

}

#endif
