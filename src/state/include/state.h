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
#include "actor/tower.h"
#include "actor/flag.h"
#include "actor/king.h"
#include "actor/base.h"
#include "actor/projectile_handler.h"
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
	 * List of Towers for each player
	 */
	std::vector<std::vector<std::shared_ptr<Tower> > > towers;
	/**
	 * Flags for each player
	 */
	std::vector<std::shared_ptr<Flag> > flags;
	/**
	 * Kings for each player
	 */
	std::vector<std::shared_ptr<King> > kings;
	/**
	 * Bases for each player
	 */
	std::vector<std::shared_ptr<Base> > bases;
	/**
	 * Handles the firing of projectiles
	 */
	ProjectileHandler projectile_handler;
	/**
	 * The State object's path planning mastermind
	 */
	PathPlanner path_planner;
	/**
	 * Contains terrain and LOS details
	 */
	Terrain terrain;
public:
	State();
	State(
		Terrain terrain,
		std::vector<std::shared_ptr<Actor> > actors
	);
	State(
		Terrain terrain,
		std::vector<std::vector<std::shared_ptr<Actor> > > sorted_actors,
		std::vector<std::shared_ptr<King> > kings,
		std::vector<std::shared_ptr<Base> > bases,
		std::vector<std::shared_ptr<Flag> > flags,
		std::vector<std::vector<std::shared_ptr<Tower> > > towers
	);
	State(
		Terrain terrain,
		std::vector<std::shared_ptr<Actor> > actors,
		std::vector<std::shared_ptr<King> > kings,
		std::vector<std::shared_ptr<Base> > bases,
		std::vector<std::shared_ptr<Flag> > flags
	);
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
	 * Gets a player's Towers
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The Towers
	 */
	std::vector<std::shared_ptr<Tower> > GetTowers(PlayerId player_id);
	/**
	 * Gets an enemy's towers, taking into account LOS
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The enemy's Towers
	 */
	std::vector<std::shared_ptr<Tower> > GetEnemyTowers(
		PlayerId player_id
	);
	/**
	 * Gets a player's Flag
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The player's Flag
	 */
	std::shared_ptr<Flag> GetFlag(PlayerId player_id);
	/**
	 * Gets the enemy's Flag
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The enemy's Flag
	 */
	std::shared_ptr<Flag> GetEnemyFlag(PlayerId player_id);
	/**
	 * Gets a player's King
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The player's King
	 */
	std::shared_ptr<King> GetKing(PlayerId player_id);
	/**
	 * Gets an enemy's King, taking into account LOS
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The enemy's King
	 */
	std::shared_ptr<King> GetEnemyKing(PlayerId player_id);
	/**
	 * Gets a player's Base
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The player's Base
	 */
	std::shared_ptr<Base> GetBase(PlayerId player_id);
	/**
	 * Gets an enemy's Base
	 *
	 * @param[in]  player_id  Player ID
	 *
	 * @return     The enemy's Base
	 */
	std::shared_ptr<Base> GetEnemyBase(PlayerId player_id);
	/**
	 * Gets the enemies in the los of a given actor
	 *
	 * @param[in]  player_id  The player ID that owns the actor
	 * @param[in]  actor_id   The actor ID
	 *
	 * @return     The enemies od the actor
	 */
	list_act_id_t GetActorEnemies(PlayerId player_id, act_id_t actor_id);
	/**
	 * Gets a player's dead units whose time_to_respawn is 0
	 *
	 * @param[in]  player_id  The player's ID
	 *
	 * @return     List of respawnables' Actor IDs
	 */
	list_act_id_t GetRespawnables(PlayerId player_id);
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
	 * Gives a player's units an attack target
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0 if attacker_ids is empty
	 * - -1 if any attacker's Actor ID is invalid
	 * - -2 if any attacking unit doesn't belong to the player who's
	 * attacking
	 * - -3 if any attacking unit is dead
	 * - -4 if the target's Actor ID is invalid
	 * - -5 if the target is in the attacking team
	 * - -6 if the target is dead
	 * - -7 if the target isn't in LOS of the attacking team
	 * - 1 if successful
	 *
	 *
	 * @param[in]  player_id         Units' player's ID
	 * @param[in]  attacker_ids      Actor IDs of the attacking units
	 * @param[in]  attack_target_id  The attack target's Actor ID
	 * @param      success           If valid pointer, holds success
	 *                               of the function
	 */
	void AttackUnit(
		PlayerId player_id,
		list_act_id_t attacker_ids,
		act_id_t attack_target_id,
		int * success
	);
	/**
	 * Instructs a player's King to capture the enemy's Flag
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0 if the King is dead
	 * - -1 if the King isn't near enough to the Flag
	 * - -2 if the King already has the Flag
	 * - 1 if successful
	 *
	 * @param[in]  player_id  King's player's ID
	 * @param      success    If valid pointer, holds success of the
	 *                        function call
	 */
	void FlagCapture(PlayerId player_id, int * success);
	/**
	 * Instructs a player's King to drop the enemy's Flag
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0 if the King is dead
	 * - -1 if the King isn't near enough to his Base
	 * - -2 if the King doesn't have a Flag
	 * - 1 if successful
	 *
	 * @param[in]  player_id  King's player's ID
	 * @param      successs   If valid pointer, holds success of the
	 *                        function call
	 */
	void FlagDrop(PlayerId player_id, int * successs);
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
	 * Sets an Actor's respawn_location if it's dead and its
	 * time_to_respawn is 0
	 *
	 * The player doing the respawning has the ID player_id
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0 if actor_id is an invalid Actor ID
	 * - -1 if the Actor to respawn isn't the player's
	 * - -2 if the Actor isn't dead
	 * - -3 if the Actor's time_to_respawn isn't 0
	 * - -4 if the respawn_location is an invalid Actor ID
	 * - -5 if the respawn_location isn't the player's
	 * - -6 if the respawn_location isn't a tower/base
	 * - 1 if successful
	 *
	 * @param[in]  player_id         ID of player the Actor belongs to
	 * @param[in]  actor_id          Actor's ID
	 * @param[in]  respawn_location  Actor ID of the base/tower it
	 *                               should respawn at
	 * @param      success           If valid pointer, holds success
	 *                               of the function call
	 */
	void RespawnUnit(
		PlayerId player_id,
		act_id_t actor_id,
		act_id_t respawn_location,
		int * success
	);
	/**
	 * Gets the terrain
	 *
	 * @return     The terrain
	 */
	const Terrain& GetTerrain() const;
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
