#ifndef STATE_ACTOR_SWORDSMAN_H
#define STATE_ACTOR_SWORDSMAN_H

#include <memory>
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Swordsman: public Actor {
public:
	Swordsman();
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
    void Update();};

}

#endif
