/**
 * @file arrow.h
 * Defines the King class
 */

#ifndef STATE_ACTOR_KING_H
#define STATE_ACTOR_KING_H

#include <memory>
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {
/**
 * Class for the king
 *
 * The king is the only unit that can captue a flag and carry it
 */
class STATE_EXPORT King: public Actor {
public:
	King();
	/**
	 * An instance of the PathPlannerHelper class to help with path planning
	 */
	PathPlannerHelper path_planner_helper;
	/**
	 * Gets the path planner helper
	 *
	 * @return     The path planner helper
	 */
	const std::unique_ptr<PathPlannerHelper> GetPathPlannerHelper();
	/**
	 * Update function to be called every tick
	 */
	void Update();
};

}

#endif
