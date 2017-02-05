/**
 * @file player_state_handler.h
 * Definitions for the state handler for players
 *
 * The state handler object via which players access the internal game
 * state.
 */

#ifndef STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H
#define STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H

#include <memory>
#include <cstdint>
#include "vector2d.h"
#include "actor/actor.h"
#include "state.h"
#include "player_state_handler/unit_views.h"
#include "terrain/terrain_element.h"
#include "state_export.h"
#include "utilities.h"

namespace state {

/**
 * Player view for TerrainElement
 *
 * @see TerrainElement
 */
struct TerrainElementView {
	physics::Vector2D position;
	int64_t size;
	TERRAIN_TYPE terrain_type;
	LOS_TYPE los_type;

	TerrainElementView();
	TerrainElementView(TerrainElement* te, PlayerId player_id);
};

/**
 * Handler for the player to access and modify the internal game state.
 *
 * Restricts player access to the internal game state. Also maintains
 * a separate copy of the state to which changes are made. The changed
 * state must be merged back periodically. Therefore, this class is
 * both a view and a buffer.
 */
class STATE_EXPORT PlayerStateHandler {
private:
	/**
	 * The PlayerStateHandler's copy of the state
	 * Will be merged with main state object every tick
	 */
	State * state;
	/**
	 * The player ID to whom the handler belongs.
	 */
	PlayerId player_id;
public:
	/**
	 * The constructor.
	 *
	 * @param[in]  state      The state
	 * @param[in]  player_id  The player identifier
	 */
	PlayerStateHandler(State * state, PlayerId player_id);
	/**
	 * Gets Actor IDs of the given player's units.
	 *
	 * @param[in]  player_id  ID of the player whose units are to be
	 *                        returned
	 *
	 * @return     List of required Actor IDs.
	 */
	list_act_id_t GetPlayerUnitIds();
	/**
	 * Gets Actor IDs for enemy units visible to a particular player
	 *
	 * @param[in]  player_id  ID of the player whose enemies are to be
	 *                        returned
	 *
	 * @return     List of required Actor IDs.
	 */
	list_act_id_t GetPlayerEnemyIds();
	/**
	 * Gets the player's Magicians
	 *
	 * @return     The player's Magicians
	 */
	std::vector<MagicianView> GetMagicians();
	/**
	 * Gets the enemy's Magicians that are in LOS
	 *
	 * @return     The enemy's Magicians
	 */
	std::vector<EnemyMagicianView> GetEnemyMagicians();
	/**
	 * Gets a player's Scouts
	 *
	 * @return     The player's Scouts
	 */
	std::vector<ScoutView> GetScouts();
	/**
	 * Gets the enemy's Scouts that are in LOS
	 *
	 * @return     The enemy's Scouts
	 */
	std::vector<EnemyScoutView> GetEnemyScouts();
	/**
	 * Gets a player's Swordsmen
	 *
	 * @return     The Swordsmen
	 */
	std::vector<SwordsmanView> GetSwordsmen();
	/**
	 * Gets an enemy's Swordsmen, taking into account LOS
	 *
	 * @return     The enemy Swordsmen
	 */
	std::vector<EnemySwordsmanView> GetEnemySwordsmen();
	/**
	 * Gets the player's Towers
	 *
	 * @return     The player's towers
	 */
	std::vector<TowerView> GetTowers();
	/**
	 * Gets the enemy's Towers
	 *
	 * @return     The enemy's towers
	 */
	std::vector<EnemyTowerView> GetEnemyTowers();
	/**
	 * Gets player's King
	 *
	 * @return     The King
	 */
	KingView GetKing();
	/**
	 * Gets the enemy's King, taking into account LOS
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0  if the enemy king is not in the LOS of the player
	 * - 1  if the king is retrieved
	 *
	 * @param      success    If valid pointer, holds success
	 *                        of the function
	 *
	 * @return     if in LOS of player, the enemy's king
	 *             else, nullptr
	 */
	EnemyKingView GetEnemyKing(int * success);
	/**
	 * Gets player's Flag
	 *
	 * @return     The Flag
	 */
	FlagView GetFlag();
	/**
	 * Gets enemy's Flag
	 *
	 * @return     The Enemy's Flag
	 */
	EnemyFlagView GetEnemyFlag();
	/**
	 * Gets the player's Base
	 *
	 * @return     The player's Base
	 */
	BaseView GetBase();
	/**
	 * Gets the enemy's Base
	 *
	 * @return     The enemy's Base
	 */
	EnemyBaseView GetEnemyBase();
	/**
	 * Gets TerrainElement corresponding to position vector
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0  if coordinate given is out of bounds
	 * - 1  if successful
	 *
	 * @param[in]  position  The position vector
	 * @param      success   If valid pointer, holds success
	 *                       of the function
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElementView CoordinateToTerrainElement(
		physics::Vector2D position,
		int * success
	);
	/**
	 * Gets TerrainElement corresponding to grid offset
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0  if coordinate given is out of bounds
	 * - 1  if successful
	 *
	 * @param[in]  offset   The position vector containing the offsets
	 *                      offset.x = row_no, offset.y = col_no
	 * @param      success  If valid pointer, holds success
	 *                      of the function
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElementView OffsetToTerrainElement(
		physics::Vector2D offset,
		int * success
	);
	/**
	 * Gets the number of rows of TerrainElements in the terrain
	 *
	 * @return     The number of rows
	 */
	int64_t GetTerrainRows();
	/**
	 * Gets the actor from it's id
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0  if actor id is invalid
	 * - -1 if actor does not belong to player
	 * - 1  if successful
	 *
	 * @param[in]  actor_id   The actor id
	 * @param      success    If valid pointer, holds success
	 *                        of the function
	 *
	 * @return     If the player_id owns the actor, the actor's UnitView
	 *             Else, returns an empty unitview
	 */
	UnitView GetUnitFromId(act_id_t actor_id, int * success);
	/**
	 * Gets the enemy actor from it's id
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0  if actor id is invalid
	 * - -1 if actor does not belong to the enemy
	 * - -2 if actor is not in LOS/is dead
	 * - 1  if successful
	 *
	 * @param[in]  actor_id   The actor id
	 * @param      success    If valid pointer, holds success
	 *                        of the function
	 *
	 * @return     If the player_id owns the actor, the actor's UnitView
	 *             Else, returns an empty EnemyUnitView
	 */
	EnemyUnitView GetEnemyUnitFromId(act_id_t actor_id, int * success);
	/**
	 * Gets the player's score
	 *
	 * @return     The score
	 */
	int64_t GetScore();
	/**
	 * Gets a player's dead units whose time_to_respawn is 0
	 *
	 * @return     List of respawnables' Actor IDs
	 */
	list_act_id_t GetRespawnables();
	/**
	 * Sets units into motion
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0  if unit_ids is empty
	 * - -1 if any unit's Actor ID is invalid
	 * - -2 if any unit doesn't belong to the player who's attacking
	 * - -3 if any unit is dead
	 * - -4 if any of the units isn't capable of moving
	 * - -5 if destination is not on the map
	 * - -6 if formation is not valid
	 * - -7 if terrain_weights isn't of size 3
	 * - -8 if terrain_weights has non-positive weights
	 * - 1  if successful
	 *
	 * @param[in]  unit_ids          Actor IDs of units to be moved
	 * @param[in]  destination       The destination
	 * @param[in]  formation_maker   The formation maker
	 * @param[in]  terrain_weights   The weights to be assigned to the
	 *                               terrain elements <Plain, Mountain, Forest>
	 * @param      path              The path the leader will move along
	 * @param      success           If valid pointer, holds success
	 *                               of the function
	 */
	void MoveUnits(
		list_act_id_t unit_ids,
		physics::Vector2D destination,
		FormationMaker * formation_maker,
		std::vector<int64_t> terrain_weights,
		std::vector<physics::Vector2D> &path,
		int * success
	);
	/**
	 * Sets units into motion
	 *
	 * The path is given by the player, no path planning is done in
	 * this method
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 *
	 * - 0 if unit_ids is empty
	 * - -1 if any unit's Actor ID is invalid
	 * - -2 if any unit doesn't belong to the player who's attacking
	 * - -3 if any unit is dead
	 * - -4 if any of the units isn't capable of moving
	 * - -5 if destinations is empty
	 * - -6 if any member of destinations is not on the map
	 * - -7 if formation is not valid
	 * - 1 if successful
	 *
	 * @param[in]  unit_ids         The unit identifiers
	 * @param[in]  destinations     The path along which the units
	 *                              should move
	 * @param[in]  formation_maker  The formation maker
	 * @param      success          The success
	 */
	void MoveUnits(
		list_act_id_t unit_ids,
		std::vector<physics::Vector2D> destinations,
		FormationMaker * formation_maker,
		int * success
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
	 * - -4 if any of the units isn't capable of attacking
	 * - -5 if the target's Actor ID is invalid
	 * - -6 if the target is in the attacking team
	 * - -7 if the target is dead
	 * - -8 if the target isn't in LOS of the attacking team
	 * - 1 if successful
	 *
	 *
	 * @param[in]  attacker_ids      Actor IDs of the attacking units
	 * @param[in]  attack_target_id  The attack target's Actor ID
	 * @param      success           If valid pointer, holds success
	 *                               of the function
	 */
	void AttackUnit(
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
	 * @param      success    If valid pointer, holds success of the
	 *                        function call
	 */
	void FlagCapture(int * success);
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
	 * @param      success    If valid pointer, holds success of the
	 *                        function call
	 */
	void FlagDrop(int * success);
	/**
	 * Calculates the total weight of the best path between
	 * the given points
	 *
	 * The parameter success's value indicates the outcome of the call
	 *
	 * success is:
	 * - 0 if start is not on the map
	 * - -1 if destination is not on the map
	 * - -2 if terrain_weights isn't of size 3
	 * - -3 if terrain_weights has non-positive weights
	 * - 1  if successful
	 *
	 * @param[in]  start        The start
	 * @param[in]  destination  The destination
	 * @param[in]  weights      The weights to be assigned to the
	 *                          terrain elements <Plain, Mountain, Forest>
	 * @param      path         The path the leader will move along
	 * @param      success      If valid pointer, holds success of the
	 *                          function call
	 *
	 * @return     The total weight of the path
	 */
	float PlanPath(
		physics::Vector2D start,
		physics::Vector2D destination,
		std::vector<int64_t> terrain_weights,
		std::vector<physics::Vector2D> &path,
		int * success
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
	 * @param[in]  actor_id          Actor's ID
	 * @param[in]  respawn_location  Actor ID of the base/tower it
	 *                               should respawn at
	 * @param      success           If valid pointer, holds success
	 *                               of the function call
	 */
	void RespawnUnit(
		act_id_t actor_id,
		act_id_t respawn_location,
		int * success
	);
};

}

#endif
