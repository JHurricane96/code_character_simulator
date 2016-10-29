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
	PathPlannerHelper path_planner_helper;
	const std::unique_ptr<PathPlannerHelper> GetPathPlannerHelper();
	void Update();
};

}

#endif
