#ifndef STATE_STATE_H
#define STATE_STATE_H

#include <vector>
#include <memory>
#include <cstdint>
#include "actor/actor.h"
#include "terrain/terrain.h"
#include "state_export.h"
#include "utilities.h"

namespace state {

struct TowerCaptureLogEntry {
	int64_t tower_id;
	int64_t timestamp;
	int64_t player_id;
};

class STATE_EXPORT State {
private:
	std::vector<const std::shared_ptr<Actor> > actors;
	std::vector<TowerCaptureLogEntry> tower_capture_log;
	list_act_id_t player_1_unit_ids, player_2_unit_ids;
	list_act_id_t player_1_visible_enemy_unit_ids, player_2_visible_enemy_unit_ids;
	list_act_id_t tower_ids;
	list_act_id_t flag_ids;
public:
	Terrain terrain;
	State();
	list_act_id_t GetPlayer1Ids();
	list_act_id_t GetPlayer2Ids();
	list_act_id_t GetPlayer1EnemyIds();
	list_act_id_t GetPlayer2EnemyIds();
	list_act_id_t GetTowerIds();
	list_act_id_t GetFlagIds();
	void Update();
};

}

#endif
