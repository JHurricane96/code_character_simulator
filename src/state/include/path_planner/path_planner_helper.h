/**
 * @file path_planner_helper.h
 * Definitions for helpers to path planning
 */

#ifndef STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H
#define STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H

#include <vector>
#include <memory>
#include "vector2d.h"
#include "actor/actor.h"
#include "state_export.h"

namespace state {

/**
 * Makes formations for path planning.
 *
 * An interface passed to path planners to make units move in
 * formation.
 */
class STATE_EXPORT Formation {
public:
	/**
	 * The ID of the formation.
	 */
	int64_t formation_id;
	/**
	 * Returns a formation.
	 *
	 * A formation is a list of positions.
	 * One position is zero, which is the reference.
	 * All other units are placed relative to the reference.
	 * This method returns a formation.
	 *
	 * @param[in] formation_size The number of units in the formation.
	 *
	 * @return The list of positions that define the formation.
	 */
	virtual std::vector<physics::Vector2D> ReturnFormation(
		int64_t formation_size
	);
};

/**
 * Helps with path planning.
 *
 * Attach this component to an Actor to allow path planning.
 * Enables fire-and-forget path planning.
 */
class STATE_EXPORT PathPlannerHelper {
private:
	/**
	 * List of points to visit in order.
	 *
	 * Once the path planner has finished, the list of points to visit
	 * is stored in this attribute.
	 */
	std::vector<physics::Vector2D> next_points;
	/**
	 * The Actor ID of the leader of the formation.
	 *
	 * @see Formation
	 */
	act_id_t leader_id;
	/**
	 * The relative position with respect to the leader of the
	 * formation.
	 *
	 * @see Formation
	 */
	physics::Vector2D relative_position;
	/**
	 * Set to true if Actor is moving in formation.
	 *
	 * @see Formation
	 */
	bool is_in_formation;
public:
	/**
	 * Determines if Actor is path planning.
	 *
	 * @return true if path planning, false otherwise.
	 */
	bool IsPathPlanning();
	/**
	 * Path planner uses this method to set path for the Actor.
	 *
	 * @param[in] points            The list of points to move to in
	 *                              order.
	 * @param[in] leader_id         The Actor ID of the leader.
	 * @param[in] relative_position The relative position of the Actor
	 *                              in the formation.
	 * @param[in] is_in_formation   Indicates if Actor should move in
	 *                              formation.
	 */
	void SetPath(
		std::vector<physics::Vector2D> next_points,
		act_id_t leader_id,
		Point relative_position,
		bool is_in_formation
	);
	/**
	 * Returns direction along which Actor should move.
	 *
	 * @param[in] current_position The current position of the Actor.
	 *
	 * @return Unit vector in the direction to move.
	 */
	physics::Vector2D ReturnDirection(
		physics::Vector2D current_position
	);
	/**
	 * Called every frame to update attributes.
	 *
	 * @param[in] current_position The current position of the Actor.
	 */
	void Update(physics::Vector2D current_position);
};

}

#endif
