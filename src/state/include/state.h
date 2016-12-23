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
#include "path_planner/path_planner.h"
#include "path_planner/path_planner_helper.h"
#include "utilities.h"
#include "state_export.h"

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
	std::vector<std::shared_ptr<Actor> > actors;
	/**
	 * List of actors sorted by x co-ordinate
	 */
	std::vector<std::vector<std::shared_ptr<Actor> > > sorted_actors;
	/**
	 * Log of tower captures
	 */
	std::vector<TowerCaptureLogEntry> tower_capture_log;
	/**
	 * List of Actor IDs belonging each player
	 */
	std::vector<list_act_id_t> player_unit_ids;
	/**
	 * List of Actor IDs of visible enemy units for each player
	 */
	std::vector<list_act_id_t> player_visible_enemy_unit_ids;
	/**
	 * List of Actor IDs for towers
	 */
	list_act_id_t tower_ids;
	/**
	 * Actor IDs for flags
	 */
	list_act_id_t flag_ids;
	/**
	 * The State object's path planning mastermind
	 */
	PathPlanner path_planner;
public:
	/**
	 * Contains terrain and LOS details
	 */
	Terrain terrain;
	State();
	/**
	 * Gets Actor IDs of the given player's units.
	 *
	 * @param[in]  player_id  ID of the player whose units are to be
	 *                        returned
	 *
	 * @return     List of required Actor IDs.
	 */
	list_act_id_t GetPlayerUnitIds(PlayerId player_id);
	/**
	 * Gets Actor IDs for enemy units visible to a particular player
	 *
	 * @param[in]  player_id  ID of the player whose enemies are to be
	 *                        returned
	 *
	 * @return     List of required Actor IDs.
	 */
	list_act_id_t GetPlayerEnemyIds(PlayerId player_id);
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
	 * Sets units into motion
	 *
	 * @param[in]  unit_ids         Actor IDs of units to be moved
	 * @param[in]  destination      The destination
	 * @param[in]  formation_maker  The formation maker
	 * @param[in]  terrain_weights  The weights to be assigned to the
	 *                              terrain elements <Plain, Mountain, Forest>
	 */
	void MoveUnits(
		list_act_id_t unit_ids,
		physics::Vector2D destination,
		std::shared_ptr<FormationMaker> formation_maker,
		std::vector<int64_t> terrain_weights
	);
	/**
	 * Sets units a target to attack
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
	 * Instructs the king to capture the flag
	 */
	void FlagCapture();
	/**
	 * Calculates the total weight of the best path between
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
	 * Gets the state
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
