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
     * @brief      Sets units into motion
     *
     * @param[in]  unit_ids        Actor IDs of units to be moved
     * @param[in]  destination     The destination
     * @param[in]  formation_type  The formation type
     */
    void MoveUnits(
        list_act_id_t unit_ids,
        physics::Vector2D destination,
        const std::unique_ptr<Formation> formation_type
    );
    /**
     * @brief      Sets units a target to attack
     *
     * @param[in]  attacker_ids      Actor IDs of player units to be
     *                               given a target
     * @param[in]  attack_target_id  The target's Actor ID
     */
    void AttackUnit(
        list_act_id_t attacker_ids,
        act_id_t attack_target_id
    );
    /**
     * @brief      Instructs the king to capture the flag
     */
    void FlagCapture();
    /**
     * @brief      Calculates the total weight of the best path between
     *             the given points
     *
     * @param[in]  start        The start
     * @param[in]  destination  The destination
     * @param[in]  weights      The weights to be assigned to the
     *                          terrain elements <Plain, Mountain, Forest>
     *
     * @return     The total weight of the path
     */
    int64_t PlanPath(
        physics::Vector2D start,
        physics::Vector2D destination,
        std::vector<int64_t> weights
        );
    /**
     * @brief      Gets the state
     *
     * @return     The State
     */
    State GetState();
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
