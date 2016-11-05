/**
 * @file archer.h
 * Defines the Archer class
 */

#ifndef STATE_ACTOR_ARCHER_H
#define STATE_ACTOR_ARCHER_H

#include <memory>
#include "actor/actor.h"
#include "actor/arrow.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {
/**
 * Class for Archers
 */
class STATE_EXPORT Archer: public Actor {
private:
	/**
	 * The max max range of an arrow shot by the archer
	 */
	int64_t range;
public:
	Archer();
	/**
	 * Gets the max range of an arrow shot by the archer
	 *
	 * @return     The range
	 */
	int64_t GetRange();
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
