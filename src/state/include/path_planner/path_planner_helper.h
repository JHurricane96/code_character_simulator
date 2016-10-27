#ifndef STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H
#define STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H

#include <vector>
#include <memory>
#include "point.h"
#include "actor/actor.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT PathPlannerHelper {
private:
	std::vector<point::Point> next_points;
	std::unique_ptr<Actor> leader;
	Point relative_position;
	bool is_in_formation;
public:
	bool IsPathPlanning();
	void SetPath(
		std::vector<point::Point> points,
		std::unique_ptr<Actor> leader,
		Point relative_position,
		bool is_in_formation
	);
};

}

#endif
