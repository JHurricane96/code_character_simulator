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
};

/**
 * Player view for PathPlannerHelper
 *
 * @see PathPlannerHelper
 */
class PathPlannerHelperView {
private:
	/**
	 * Pointer to underlying PathPlannerHelper instance which the view
	 * provides an interface for.
	 */
	std::shared_ptr<PathPlannerHelper> path_planner_helper;
public:
	bool IsPathPlanning();
};

/**
 * Player view for Actors that are movable.
 *
 * @see Actor
 */
class UnitView {
private:
	/**
	 * Pointer to underlying Actor instance which the view provides an
	 * interface for.
	 */
	std::shared_ptr<Actor> unit;
public:
	/**
	 * Gets the ID of the actor
	 *
	 * @return     The ID
	 */
	act_id_t GetId();
	/**
	 * Gets the health of the actor
	 *
	 * @return     The hp
	 */
	int64_t GetHp();
	/**
	 * Gets the maximum speed possible for the actor
	 *
	 * @return     The maximum speed
	 */
	int64_t GetMaxSpeed();
	/**
	 * Gets the ID of the actor's target
	 *
	 * @return     The target's ID
	 */
	act_id_t GetAttackTargetId();
	/**
	 * Gets the velocity vector of the actor
	 *
	 * @return     The velocity vector
	 */
	physics::Vector2D GetVelocity();
	/**
	 * Gets the position vector of the actor
	 *
	 * @return     The position vector
	 */
	physics::Vector2D GetPosition();
	/**
	 * Gets the path planner helper
	 *
	 * @return     The path planner helper
	 */
	PathPlannerHelperView GetPathPlannerHelper();
	/**
	 * Sets the unit on course to attack the enemy unit specified by the attack_target_id
	 *
	 * @param[in]  attack_target_id  The actor's attack target
	 */
	void SetAttackTargetId(act_id_t attack_target_id);
};

/**
 * Provides a restricted view of the enemy units to the player
 * Given the enemy Actor ID, the player can view the position of the unit
 */
class EnemyUnitView {
private:
	/**
	 * Pointer to underlying Actor instance which the view provides an
	 * interface for.
	 */
	std::shared_ptr<Actor> unit;
public:
	/**
	 * Gets the ID of the enemy
	 *
	 * @return     The ID
	 */
	act_id_t GetId();
	/**
	 * Gets the position vector of the enemy
	 *
	 * @return     The position vector
	 */
	physics::Vector2D GetPosition();
};


/**
 * Player view for the State.
 *
 * @see State
 */
class STATE_EXPORT StateView {
private:
	/**
	 * Pointer to underlying State instance which the view provides an
	 * interface for.
	 */
	std::shared_ptr<State> state;
public:
	StateView(State &state);
	/**
	 * Gets Actor IDs for player units.
	 *
	 * @return     The unit Actor IDs
	 */
	list_act_id_t GetUnitIds();
	/**
	 * Gets Actor IDs for enemy units.
	 *
	 * @return     The enemy Actor IDs
	 */
	list_act_id_t GetEnemyIds();
	/**
	 * Gets Tower IDs the player has explored
	 *
	 * @return     The tower IDs
	 */
	list_act_id_t GetTowerIds();
	/**
	 * Gets Flag IDs
	 *
	 * @return     The flag IDs
	 */
	list_act_id_t GetFlagIds();
	/**
	 * Gets TerrainElement corresponding to position vector
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElementView XYToTerrainElement();
	/**
	 * Gets TerrainElement corresponding to position offset
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElementView OffsetToTerrainElement();
	/**
	 * Gets the player's unit from Actor ID
	 *
	 * @param[in]  actor_id  The actor ID
	 *
	 * @return     Corresponding UnitVIew
	 */
	UnitView GetUnitFromId(act_id_t actor_id);
	/**
	 * Gets the player's enemy unit from Actor ID
	 *
	 * @param[in]  actor_id  The actor ID
	 *
	 * @return     Corresponding EnemyUnitView
	 */
	EnemyUnitView GetEnemyUnitFromId(act_id_t actor_id);
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
	 * A restricted view of the state with appropriate getters.
	 */
	const StateView state_view;
	/**
	 * The player ID to whom the handler belongs.
	 */
	act_id_t player_id;
public:
	/**
	 * The constructor.
	 *
	 * @param[in] state The state object to create a view for.
	 */
	PlayerStateHandler(State &state);
	/**
	 * Sets units into motion
	 *
	 * @param[in]  unit_ids         Actor IDs of units to be moved
	 * @param[in]  destination      The destination
	 * @param[in]  formation_maker  The formation maker
	 * @param[in]  terrain_weights  Weights determining terrain
	 *                              preference while path finding
	 */
	void MoveUnits(
		list_act_id_t unit_ids,
		physics::Vector2D destination,
		const std::unique_ptr<FormationMaker> formation_maker,
		std::vector<int64_t> terrain_weights
	);
	/**
	 * Sets units a target to attack
	 * Calls the PathPlanner for each unit to set the path the units
	 * would follow
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
	 *
	 * Calls the PathPlanner to set the path the king would follow
	 */
	void FlagCapture();
	/**
	 * Calculates the total weight of the best path between the given points
	 *
	 * @param[in]  start            The start
	 * @param[in]  destination      The destination
	 * @param[in]  terrain_weights  Weights determining terrain
	 *                              preference while path finding
	 *
	 * @return     The total weight of the path
	 */
	int64_t PlanPath(
		physics::Vector2D start,
		physics::Vector2D destination,
		std::vector<int64_t> terrain_weights
	);
	/**
	 * Gets the restricted copy of the state for the player to examine
	 *
	 * @return     The StateView
	 */
	StateView GetState();
};

}

#endif
