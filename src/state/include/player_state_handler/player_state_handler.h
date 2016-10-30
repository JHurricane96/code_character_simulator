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
	physics::Vector2D GetPosition();
	act_id_t GetId();
	int64_t GetHp();
	int64_t GetSpeed();
	act_id_t GetAttackTargetId();
	physics::Vector2D GetVelocity();
	physics::Vector2D GetPosition();
	PathPlannerHelperView GetPathPlannerHelper();
	void SetAttackTargetId(act_id_t attack_target_id);
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
	list_act_id_t GetUnitIds();
	list_act_id_t GetEnemyIds();
	list_act_id_t GetTowerIds();
	list_act_id_t GetFlagIds();
	TerrainElementView XYToTerrainElement();
	TerrainElementView OffsetToTerrainElement();
	UnitView GetUnitFromId();
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
	void MoveUnits(
		list_act_id_t unit_ids,
		physics::Vector2D destination,
		const std::unique_ptr<Formation> formation_type
	);
	void AttackUnit(
		list_act_id_t attacker_ids,
		act_id_t attack_target_id
	);
	int64_t PlanPath();
	StateView GetState();
};

}

#endif
