/**
 * @file state.h
 * Definitions for state object
 * 
 * Definitions for the complete internal state of the game
 */

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

/**
 * Log entry for tower captures
 */
struct TowerCaptureLogEntry {
	/**
	 * Tower ID that entry refers to
	 */
	int64_t tower_id;
	/**
	 * UTC timestamp of capture
	 */
	int64_t timestamp;
	/**
	 * ID of player that entry refers to
	 */
	int64_t player_id;
};

/**
 * Internal state of the simulation
 */
class STATE_EXPORT State {
private:
	/**
	 * List of actors in the simulation
	 */
	std::vector<const std::shared_ptr<Actor> > actors;
	/**
	 * Log of tower captures
	 */
	std::vector<TowerCaptureLogEntry> tower_capture_log;
	/**
	 * List of Actor IDs belonging each player
	 */
	list_act_id_t player_1_unit_ids, player_2_unit_ids;
	/**
	 * List of Actor IDs of visible enemy units for each player
	 */
	list_act_id_t
		player_1_visible_enemy_unit_ids,
		player_2_visible_enemy_unit_ids;
	/**
	 * List of Actor IDs for towers
	 */
	list_act_id_t tower_ids;
	/**
	 * Actor IDs for flags
	 */
	list_act_id_t flag_ids;
public:
	/**
	 * Contains terrain and LOS details
	 */
	Terrain terrain;
	State();
	/**
	 * Gets Actor IDs for player 1 units.
	 *
	 * @return List of required Actor IDs.
	 */
	list_act_id_t GetPlayer1Ids();
	/**
	 * Gets Actor IDs for player 2 units.
	 *
	 * @return List of required Actor IDs.
	 */
	list_act_id_t GetPlayer2Ids();
	/**
	 * Gets Actor IDs for enemy units visible to player 1.
	 *
	 * @return List of required Actor IDs.
	 */
	list_act_id_t GetPlayer1EnemyIds();
	/**
	 * Gets Actor IDs for enemy units visible to player 2.
	 *
	 * @return List of required Actor IDs.
	 */
	list_act_id_t GetPlayer2EnemyIds();
	/**
	 * Gets Actor IDs for towers.
	 *
	 * @return List of required Actor IDs.
	 */
	list_act_id_t GetTowerIds();
	/**
	 * Gets Actor IDs for flags.
	 *
	 * @return List of required Actor IDs.
	 */
	list_act_id_t GetFlagIds();
	/**
	 * Updates the internal state of the game.
	 * 
	 * @param[in] delta_time The difference in time in ms between
	 *                       the previous and current Update calls.
	 */
	void Update(int64_t delta_time);
};

}

#endif
