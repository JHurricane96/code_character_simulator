#ifndef STATE_ACTOR_ARCHER_H
#define STATE_ACTOR_ARCHER_H

#include <memory>
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {
/**
 * @brief      Class for Archers
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
     * @brief      Gets the max range of an arrow shot by the archer
     *
     * @return     The range
     */
	int64_t GetRange();
    /**
     * An instance of the PathPlannerHelper class to help with path planning
     */
	PathPlannerHelper path_planner_helper;
    /**
     * @brief      Gets the path planner helper
     *
     * @return     The path planner helper
     */
	const std::unique_ptr<PathPlannerHelper> GetPathPlannerHelper();
    /**
     * @brief      Update function to be called every tick
     */
	void Update();
};

}

#endif
