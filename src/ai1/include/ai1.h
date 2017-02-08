#ifndef AI1_AI1_H
#define AI1_AI1_H

#include <memory>
#include "player_ai_helper.h"
#include "ai1_export.h"

namespace ai1 {

class TogetherFormation;

class AI1_EXPORT Ai1 : public player::PlayerAiHelper {
private:
	bool init = false;

	bool lol = false;

	int64_t terrain_elt_size;

	state::list_act_id_t def_ids;
	int def_mode = 0;
	state::act_id_t def_att_target = -1;

	state::list_act_id_t att_ids;
	int att_mode = 0;

	std::vector<physics::Vector2D> patrol_path;
	int cur_patrol_pnt = 0;
	int hello;

	std::unique_ptr<state::FormationMaker> together_formation;
public:
	void Update(std::shared_ptr<state::PlayerStateHandler> state) override;
};

}

#endif
