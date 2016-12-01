/**
 * @file formation.h
 * Defines formations that handle coordinated unit movement
 */
#ifndef PATH_PLANNER_FORMATION_H
#define PATH_PLANNER_FORMATION_H

#include <vector>
#include <memory>
#include <cstdint>
#include "vector2d.h"
#include "utilities.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {

/**
 * Used to handle coordinated unit movement
 */
class STATE_EXPORT Formation {
private:
	/**
	 * ID of the player that owns the formation
	 */
	PlayerId player_id;
	/**
	 * ID of this formation
	 */
	int64_t formation_id;
	/**
	 * Units in the formation
	 */
	std::vector<shared_ptr<Actor> > units;
	/**
	 * Leader of the formation
	 * 
	 * All units' positions are relative to the leader
	 */
	std::shared_ptr<Actor> leader;
	/**
	 * Formation maker that returns list of points that define the
	 * formation
	 */
	std::shared_ptr<FormationMaker> formation_maker;
	/**
	 * List of destinations to visit in order
	 */
	std::vector<physics::Vector2D> destinations;
public:
	/**
	 * Creates a new Formation
	 *
	 * @param[in]  player_id        ID of the player that owns the
	 *                              formation
	 * @param[in]  formation_id     ID of the formation
	 * @param[in]  units            Units in the formation
	 * @param[in]  formation_maker  The formation maker
	 * @param[in]  destinations     The destinations
	 */
	Formation(
		PlayerId player_id,
		int64_t formation_id,
		std::vector<std::shared_ptr<Actor> > units,
		std::shared_ptr<FormationMaker> formation_maker,
		std::vector<physics::Vector2D> destinations
	);
	/**
	 * Update function to be called every tick
	 * 
	 * Sets the velocity of each unit in the formation
	 *
	 * @param      sorted_allies   Ally units sorted by x co-ordinate
	 * @param      sorted_enemies  Enemy units sorted by x co-ordinate
	 */
	Update(
		std::vector<std::shared_ptr<Actor> > &sorted_allies,
		std::vector<std::shared_ptr<Actor> > &sorted_enemies
	);
}

}

#endif
