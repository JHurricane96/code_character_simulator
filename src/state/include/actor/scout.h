/**
 * @file scout.h
 * Defines the Scout class
 */

#ifndef STATE_ACTOR_SCOUT_H
#define STATE_ACTOR_SCOUT_H

#include <memory>
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {
/**
 * Class for scout
 *
 * The scout cannot attack and is invisible to the enemy unless he is
 * within the line of site of the base
 */
class STATE_EXPORT Scout: public Actor {
public:
	Scout();
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
