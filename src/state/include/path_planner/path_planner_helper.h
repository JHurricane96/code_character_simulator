/**
 * @file path_planner_helper.h
 * Definitions for helpers to path planning
 */

#ifndef STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H
#define STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H

#include <vector>
#include <memory>
#include "vector2d.h"
#include "actor/actor.fwd.h"
#include "state_export.h"

namespace state {

/**
 * Makes formations for path planning
 *
 * An interface passed to path planners to make units move in
 * formation
 */
class STATE_EXPORT FormationMaker {
public:
	/**
	 * The ID of the formation
	 */
	int64_t formation_id;
	FormationMaker();
	/**
	 * Returns a formation
	 *
	 * A formation is a list of positions
	 * One position is zero, which is the reference
	 * All other units are placed relative to the reference
	 * This method returns a formation
	 *
	 * @param[in] formation_size The number of units in the formation
	 *
	 * @return The list of positions that define the formation
	 */
	virtual std::vector<physics::Vector2D> ReturnFormation(
		int64_t formation_size
	) = 0;
};

/**
 * Determines if a formation described by the FormationMaker class
 * is valid
 *
 * @param[in]  formation_maker  The formation maker instance
 * @param[in]  formation_size   The formation size
 *
 * @return     True if formation is valid, False otherwise
 */
bool IsValidFormation(
	FormationMaker * formation_maker,
	int64_t formation_size
);


/**
 * Helps with path planning
 *
 * Attach this component to an Actor to allow path planning
 * Enables fire-and-forget path planning
 */
class STATE_EXPORT PathPlannerHelper {
private:
	/**
	 * The Actor that owns this PathPlannerHelper
	 */
	std::weak_ptr<Actor> self;
	/**
	 * The leader of the formation to which this unit belongs
	 */
	std::shared_ptr<Actor> leader;
	/**
	 * The ID of the formation to which this units belongs
	 */
	int64_t formation_id;
	/**
	 * The relative position of this unit with respect to the leader
	 * of the formation
	 */
	physics::Vector2D relative_position;
	/**
	 * Set to true if the unit is currently moving
	 */
	bool is_path_planning;
	/**
	 * True when the unit is in the right place in the formation
	 *
	 * Always true for leaders
	 */
	bool is_in_formation;
	/**
	 * The formation leader's destination
	 */
	physics::Vector2D leader_destination;
public:
	/**
	 * Constructor for PathPlannerHelper
	 *
	 * @param[in]  self  Actor that owns this PathPlannerHelper
	 */
	PathPlannerHelper();
	PathPlannerHelper(std::shared_ptr<Actor> self);
	/**
	 * Sets a path for this unit
	 *
	 * Used when a unit is to be added to a formation
	 *
	 * @param[in]  formation_id       ID of the formation to which
	 *                                this unit shall belong
	 * @param[in]  relative_position  Position of this unit with
	 *                                respect to the leader
	 * @param[in]  is_leader          Indicates if leader of formation
	 * @param[in]  leader             The leader of the formation.
	 *                                Valid only if is_leader is set
	 */
	void SetPath(
		int64_t formation_id,
		physics::Vector2D relative_position,
		bool is_leader,
		std::shared_ptr<Actor> leader
	);
	/**
	 * Sets is_path_planning to false
	 *
	 * Called when this unit reaches its final destination
	 */
	void FinishPath();
	/**
	 * Gets unit's formation's ID
	 *
	 * @return     The formation ID
	 */
	int64_t GetFormationId();
	/**
	 * Determines if this unit is a formation leader
	 *
	 * @return     true if leader, false otherwise
	 */
	bool IsLeader();
	/**
	 * Updates this unit's formation leader
	 *
	 * Used when the old leader is dead/in another formation
	 *
	 * @param[in]  new_leader  The new leader
	 */
	void UpdateLeader(std::shared_ptr<Actor> new_leader);
	/**
	 * Makes this unit its formation's leader
	 *
	 * Used when the old leader is dead/in another formation
	 */
	void MakeLeader();
	/**
	 * Updates this unit's relative position in its formation
	 *
	 * Used after some units leave the formation
	 *
	 * @param[in]  relative_position  The new relative position
	 */
	void UpdateRelativePosition(physics::Vector2D relative_position);
	/**
	 * Gets the unit's current destination
	 *
	 * If leader, returns leader_destination.
	 * Else, returns leader's position + relative_position
	 *
	 * @return     The destination
	 */
	physics::Vector2D GetDestination();
	/**
	 * Determines if unit is moving
	 *
	 * @return     true if moving, false otherwise.
	 */
	bool IsPathPlanning();
	/**
	 * Determines if the unit is in formation
	 *
	 * @return     true if in formation, false otherwise
	 */
	bool IsInFormation();
	/**
	 * Update function to be called every tick
	 *
	 * Sets the velocity of this unit
	 *
	 * @param      sorted_allies   Ally units sorted by x co-ordinate
	 * @param      sorted_enemies  Enemy units sorted by x co-ordinate
	 * @param[in]  destination     The destination
	 * @param[in]  speed           The speed with which this unit should travel
	 */
	void Update(
		std::vector<std::shared_ptr<Actor> > &sorted_allies,
		std::vector<std::shared_ptr<Actor> > &sorted_enemies,
		physics::Vector2D destination,
		float speed
	);
	/**
	 * Merge this, a PathPlannerHelper in the main state, with the
	 * corresponding one in a player's state
	 *
	 * @param[in]  path_planner_helper  The player's PathPlannerHelper
	 * @param[in]  actors               The main state's Actors. Any references
	 *                                  to the other state's Actors are replaced
	 *                                  by this state's Actors
	 */
	void MergeWithBuffer(
		const PathPlannerHelper& path_planner_helper,
		std::vector<std::shared_ptr<Actor> > actors
	);
};

}

#endif
