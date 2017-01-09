/**
 * @file vnit_views.h
 *
 * Definitions for the restricted unit views for players
 */

#ifndef STATE_PLAYER_STATE_HANDLER_UNIT_VIEWS_H
#define STATE_PLAYER_STATE_HANDLER_UNIT_VIEWS_H

#include "actor/actor.h"
#include "vector2d.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"
#include "utilities.h"
#include <cstdint>

namespace state {

/**
 * Player view for PathPlannerHelper
 *
 * @see PathPlannerHelper
 */
class STATE_EXPORT PathPlannerHelperView {
private:
	/**
	 * Pointer to underlying PathPlannerHelper instance which the view
	 * provides an interface for.
	 */
	PathPlannerHelper * path_planner_helper;
public:
	PathPlannerHelperView(PathPlannerHelper * path_planner_helper);
	/**
	 * Determines if the unit is path planning
	 *
	 * @return     True if path planning, False otherwise.
	 */
	bool IsPathPlanning();
};

/**
 * Provides a restricted view of the enemy units to the player
 * Given the enemy Actor ID, the player can view the position of the unit
 */
class STATE_EXPORT EnemyUnitView {
private:
	/**
	 * Pointer to underlying Actor instance which the view provides an
	 * interface for.
	 */
	Actor * unit;
	public:
	EnemyUnitView();
	EnemyUnitView(Actor * unit);
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
	/**
	 * Gets the Actor's type
	 *
	 * @return     The Actor's type
	 */
	ActorType GetActorType();
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
	Actor * unit;
public:
	UnitView();
	UnitView(Actor * actor);
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
	 * Gets the Actor's type
	 *
	 * @return     The Actor's type
	 */
	ActorType GetActorType();
	/**
	 * Gets the actor's target
	 *
	 * @return     The restricted view of the enemy
	 */
	EnemyUnitView GetAttackTarget();
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
};

}

#endif
