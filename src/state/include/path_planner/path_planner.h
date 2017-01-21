/**
 * @file path_planner.h
 * The path planning mastermind
 *
 * Contains the path planning mastermind that takes care of
 * moving in formations
 */

#ifndef STATE_PATH_PLANNER_PATH_PLANNER_H
#define STATE_PATH_PLANNER_PATH_PLANNER_H

#include <vector>
#include <memory>
#include <cstdint>
#include "vector2d.h"
#include "utilities.h"
#include "terrain/terrain.h"
#include "actor/actor.h"
#include "path_planner/formation.h"
#include "path_planner/graph.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {

/**
 * The path planning mastermind
 * 
 * Stores list of formations, plans their paths and updates them
 */
class STATE_EXPORT PathPlanner {
private:
	/**
	 * List of formations on the field
	 */
	std::vector<std::vector<Formation> > formations;
	/**
	 * Formation ID of the next formation to be made
	 * 
	 * Incremented when a formation is made
	 */
	std::vector<int64_t> next_formation_id;
	/**
	 * Graph that helps with path planning
	 * 
	 * Does the actual path finding
	 */
	Graph graph;
public:
	/**
	 * Constructor for PathPlanner
	 *
	 * @param[in]  map_size  The map size
	 */
	PathPlanner(int64_t map_size);
	/**
	 * Makes a formation
	 *
	 * The formation is a list of actors that need to move around the
	 * map in formation. Called when a bunch of units needs to be moved
	 *
	 * @param[in]  player_id        ID of the player that will own the
	 *                              formation
	 * @param      units            Units in the formation
	 * @param      terrain          The current status of the map to be
	 *                              used for path planning
	 * @param[in]  formation_maker  Returns list of points that defines
	 *                              the formation layout
	 * @param[in]  destination      The destination
	 * @param[in]  terrain_weights  Weights determining terrain
	 *                              preference while path finding
	 * @param      path             The path the leader will move along
	 */
	void MakeFormation(
		PlayerId player_id,
		std::vector<std::shared_ptr<Actor> > &units,
		Terrain &terrain,
		std::shared_ptr<FormationMaker> formation_maker,
		physics::Vector2D destination,
		std::vector<int64_t> terrain_weights,
		std::vector<physics::Vector2D> &path
	);
	/**
	 * Plans a path
	 *
	 * @param[in]  start_point      The start point
	 * @param[in]  destination      The destination
	 * @param      terrain          The terrain
	 * @param[out] next_points      Ordered list of points to visit to
	 *                              reach the destination
	 * @param[in]  terrain_weights  Weights determining terrain
	 *                              preference while path finding
	 *
	 * @return     The weight of the path
	 */
	float PlanPath(
		physics::Vector2D start_point,
		physics::Vector2D destination,
		Terrain &terrain,
		std::vector<physics::Vector2D> &next_points,
		std::vector<int64_t> terrain_weights
	);
	/**
	 * Update function to be called every tick
	 *
	 * Sets the units' velocities in each formation
	 *
	 * @param      sorted_units  Units sorted by x co-ordinate
	 */
	void Update(
		std::vector<std::vector<std::shared_ptr<Actor> > > &sorted_units
	);
	/**
	 * Merges this, a PathPlanner in the main State, with the
	 * PathPlanner in a player's state
	 *
	 * @param[in]  path_planner  The player's PathPlanner
	 * @param[in]  player_id     The ID of the PathPlanner's player
	 * @param[in]  actors        The main state's Actors. Any references
	 *                           to the other state's Actors are replaced
	 *                           by this state's Actors
	 */
	void MergeWithBuffer(
		const PathPlanner& path_planner,
		PlayerId player_id,
		std::vector<std::shared_ptr<Actor> > actors
	);
	/**
	 * Merges this, a PathPlanner in a player's state, the PathPlanner
	 * in the main state
	 *
	 * @param[in]  path_planner  The main state's PathPlanner
	 * @param[in]  actors        The player's state's Actors. Any
	 *                           references to the other state's Actors
	 *                           are replaced by this state's Actors
	 */
	void MergeWithMain(
		const PathPlanner& path_planner,
		std::vector<std::shared_ptr<Actor> > actors
	);
};

}

#endif
