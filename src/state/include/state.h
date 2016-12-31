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
		std::vector<std::shared_ptr<Flag> > flags
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
	 * Gets all visible enemies of a player
	 *
	 * @param[in]  player_id  The player ID of the player whose enemies
	 * 						  are to be found
	 *
	 * @return     All visible enemies
	 */
	list_act_id_t GetAllVisibleEnemies(PlayerId player_id);
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
	 * Instructs a player's King to capture the enemy's Flag
	 *
	 * Fails if the King is not near enough the Flag, if he has it
	 * already, or he's dead
	 *
	 * @param[in]  player_id  King's player's ID
	 * @param      success    Holds 1 if successful, 0 if he's too far
	 *                        away, -1 if he already has a flag
	 */
	void FlagCapture(PlayerId player_id, int * success);
	/**
	 * Instructs a player's King to drop the enemy's Flag
	 *
	 * Fails if the King is not near enough his Base, if he doesn't have
	 * the Flag, or if he's dead
	 *
	 * @param[in]  player_id  King's player's ID
	 * @param      successs   Holds 1 if successful, 0 if he's too far
	 *                        from his Base, -1 if he doesn't have the
	 *                        Flag
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
